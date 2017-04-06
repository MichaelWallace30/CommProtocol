/*
Serial configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(At your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H
#include <CommProto/architecture/api.h>
#include <CommProto/serialization/marshal.h>
#include <CommProto/serialization/objserializable.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/headerpacket.h>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <string>

namespace comnet {
	namespace serialization {
		/**
			Object Stream Class will hold an array of data as a stream.
			All data will be put into the stream with the overloaded operators '<<'
			All data will be taken out of the stream with the overloaded operators '>>'
			The Stream is a stack Last In First Out (LIFO).
			Therefore reverse the order data is put in when taking out.
			To input a char* buffer you can call setBuffer(buffer, length).
			Setting the buffer will erase information by setting current postion to new size of buffer.
			Inputing a buffer is used on the lower level when dealing directly with a data link.
		*/
		class COMM_EXPORT ObjectStream {
			COMM_DISALLOW_COPYING(ObjectStream);

		private:
			/** Data stream to hold input as an array of bytes*/
			marshal_t stream_buffer;

			/** Current postion of streamBuffer and also size of streamBuffer*/
			int32_t curr_pos;
			/** Error Handingling methods*/
			void PrintErrorUnderFlow() { COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer too small\n"); }
			/** Error Handingling methods*/
			void PrintErrorOverFlow() { COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer full\n"); }

		public:

			/** Header packet for communication*/
			Header header_packet;

			/** Constructor*/
			ObjectStream();

			// Move constructions.
			ObjectStream(ObjectStream&& obj)
				: stream_buffer(nullptr)
				, curr_pos(0)
				, header_packet(Header()) {
				std::swap(stream_buffer, obj.stream_buffer);
				std::swap(curr_pos, obj.curr_pos);
				std::swap(header_packet, obj.header_packet);
			}

			ObjectStream& operator=(ObjectStream&& obj) {
				std::swap(obj.stream_buffer, stream_buffer);
				std::swap(obj.curr_pos, curr_pos);
				std::swap(obj.header_packet, header_packet);
				return *this;
			}
			/** Destructor*/
			~ObjectStream();
			/** getPostion returns the postion of the stream buffer*/
			int32_t GetPosition() const { return curr_pos; }
			/** getSize return the size of the current buffer*/
			int32_t GetSize() const { return curr_pos; }
			/** getBuffer returns a pionter to the buffer stream*/
			marshal_t GetBuffer() const { return stream_buffer; }

			Header& GetHeaderPacket() { return header_packet; }
			/** setBuffer will take a pointer to an array of char and its len to become the new stream of data*/
			void SetBuffer(const char* buffer, int len);

			void SetHeader(Header& header);

			void SerializeHeader();
			Header DeserializeHeader();

			/** Overloaded input stream operators to put variables into the object stream*/
			ObjectStream& operator<<(const string_t& data);
			ObjectStream& operator<<(const std::wstring& data);
			ObjectStream& operator<<(const std::string& data);
			ObjectStream& operator<<(const uint8_t& data);
			ObjectStream& operator<<(const int8_t& data);
			ObjectStream& operator<<(const uint16_t& data);
			ObjectStream& operator<<(const int16_t& data);
			ObjectStream& operator<<(const int32_t& data);
			ObjectStream& operator<<(const uint32_t& data);
			ObjectStream& operator<<(const int64_t& data);
			ObjectStream& operator<<(const uint64_t& data);
			ObjectStream& operator<<(const real32_t& data);
			ObjectStream& operator<<(const real64_t& data);

			template <typename T>
			ObjectStream& operator<<(const std::vector<T>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//list, multiset, unordered_multiset compatible
			template <typename T>
			ObjectStream& operator<<(const std::vector<T*>& data)
			{
				uint16_t size = data.size();
				if (size > 0) {
					std::unordered_map<T*, uint16_t> elmIndexMap;
					uint16_t i = 0;
					for (; i < size; i++) {
						auto mapIter = elmIndexMap.find(data.at(i));
						if (mapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(data.at(i), i));
						}
					}
					i--;
					for (; i >= 0; i--) {
						auto mapIter = elmIndexMap.find(data.at(i));
						if (mapIter->second == i) {
							*this << *mapIter->first;
						}
						*this << mapIter->second;
						if (i == 0) {
							break;
						}
					}
				}
				*this << size;
				return *this;
			}

			template <typename T>
			ObjectStream& operator<<(const std::list<T>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//Vector, multiset, multiunordered_set compatible
			template <typename T>
			ObjectStream& operator<<(const std::list<T*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map<T*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t i = 0;
					uint16_t elmI = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(*it, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter->second.first == i) {
							*this << *mapIter->first;
						}
						*this << mapIter->second.second;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T>
			ObjectStream& operator<<(const std::set<T>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//unordered_set compatible
			template <typename T>
			ObjectStream& operator<<(const std::set<T*>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++)
				{
					*this << **it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T>
			ObjectStream& operator<<(const std::multiset<T>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//vector,list,unordered_multiset compatible
			template <typename T>
			ObjectStream& operator<<(const std::multiset<T*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map<T*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t i = 0;
					uint16_t elmI = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(*it, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter->second.first == i) {
							*this << *mapIter->first;
						}
						*this << mapIter->second.second;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T>
			ObjectStream& operator<<(const std::unordered_set<T>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//set compatible
			template <typename T>
			ObjectStream& operator<<(const std::unordered_set<T*>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++)
				{
					*this << **it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T>
			ObjectStream& operator<<(const std::unordered_multiset<T>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++)
				{
					*this << *it;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			//vector, list, multiset compatible
			template <typename T>
			ObjectStream& operator<<(const std::unordered_multiset<T*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map<T*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t i = 0;
					uint16_t elmI = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(*it, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter->second.first == i) {
							*this << *mapIter->first;
						}
						*this << mapIter->second.second;
						if (it == data.begin()) {
							break;
						}
						it--;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator<<(const std::map<T, D>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++) {
					*this << it->second;
					*this << it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::map<T, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::map<T*, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << *it->first;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator<<(const std::multimap<T, D>& data)
			{
				for (auto it = data.rbegin(); it != data.rend(); it++) {
					*this << it->second;
					*this << it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::multimap<T, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::multimap<T*, D>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <T*, std::pair<uint16_t, uint16_t>> keyIndexMap;
					uint16_t keyI = 0;
					uint16_t i = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter == keyIndexMap.end()) {
							keyIndexMap.emplace(std::make_pair(it->first, std::make_pair(i, keyI)));
							keyI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						*this << it->second;
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter->second.first == i) {
							*this << *keyMapIter->first;
						}
						*this << keyMapIter->second.second;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::multimap<T*, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <T*, std::pair<uint16_t, uint16_t>> keyIndexMap;
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> valIndexMap;
					uint16_t keyI = 0;
					uint16_t valI = 0;
					uint16_t i = 0;
					for (auto it = data.begin(); it != data.end(); it++) {
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter == keyIndexMap.end())
						{
							keyIndexMap.emplace(std::make_pair(it->first, std::make_pair(i, keyI)));
							keyI++;
						}
						auto valMapIter = valIndexMap.find(it->second);
						if (valMapIter == valIndexMap.end()) {
							valIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, valI)));
							valI++;
						}
						i++;
					}
					i--;
					for (auto it = data.rbegin(); it != data.rend(); it++) {
						auto valMapIter = valIndexMap.find(it->second);
						if (valMapIter->second.first == i) {
							*this << *valMapIter->first;
						}
						*this << valMapIter->second.second;
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter->second.first == i) {
							*this << *keyMapIter->first;
						}
						*this << *keyMapIter->second.second;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator<<(const std::unordered_map<T, D>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++) {
					*this << it->second;
					*this << it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::unordered_map<T, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
						if (it == data.begin()) {
							break;
						}
						it--;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::unordered_map<T*, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << *it->first;
						i--;
						if (it == data.begin()) {
							break;
						}
						it--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator<<(const std::unordered_multimap<T, D>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++) {
					*this << it->second;
					*this << it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::unordered_multimap<T, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> elmIndexMap;
					uint16_t elmI = 0;
					uint16_t i = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter == elmIndexMap.end()) {
							elmIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, elmI)));
							elmI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
						if (it == data.begin()) {
							break;
						}
						it--;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::unordered_multimap<T*, D>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <T*, std::pair<uint16_t, uint16_t>> keyIndexMap;
					uint16_t keyI = 0;
					uint16_t i = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter == keyIndexMap.end()) {
							keyIndexMap.emplace(std::make_pair(it->first, std::make_pair(i, keyI)));
							keyI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						*this << it->second;
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter->second.first == i) {
							*this << *keyMapIter->first;
						}
						*this << keyMapIter->second.second;
						if (it == data.begin()) {
							break;
						}
						it--;
						i--;
					}
				}
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator <<(const std::unordered_multimap<T*, D*>& data)
			{
				uint16_t size = data.size();
				if (size > 0)
				{
					std::unordered_map <T*, std::pair<uint16_t, uint16_t>> keyIndexMap;
					std::unordered_map <D*, std::pair<uint16_t, uint16_t>> valIndexMap;
					uint16_t keyI = 0;
					uint16_t valI = 0;
					uint16_t i = 0;
					auto it = data.begin();
					for (; it != data.end(); it++) {
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter == keyIndexMap.end())
						{
							keyIndexMap.emplace(std::make_pair(it->first, std::make_pair(i, keyI)));
							keyI++;
						}
						auto valMapIter = valIndexMap.find(it->second);
						if (valMapIter == valIndexMap.end()) {
							valIndexMap.emplace(std::make_pair(it->second, std::make_pair(i, valI)));
							valI++;
						}
						i++;
					}
					i--;
					it--;
					while (true) {
						auto valMapIter = valIndexMap.find(it->second);
						if (valMapIter->second.first == i) {
							*this << *valMapIter->first;
						}
						*this << (uint16_t)valMapIter->second.second;
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter->second.first == i) {
							*this << *keyMapIter->first;
						}
						*this << (uint16_t)keyMapIter->second.second;
						if (it == data.begin()) {
							break;
						}
						i--;
						it--;
					}
				}
				*this << size;
				return *this;
			}

			/** OVerloaded output stream operators to output variables to a variable from the object stream
				string_t (char*) must use malloc when inputing data into new c-string variable*/
			ObjectStream& operator>>(string_t& data);
			ObjectStream& operator>>(std::wstring& data);
			ObjectStream& operator>>(std::string& data);
			ObjectStream& operator>>(uint8_t& data);
			ObjectStream& operator>>(int8_t& data);
			ObjectStream& operator>>(uint16_t& data);
			ObjectStream& operator>>(int16_t& data);
			ObjectStream& operator>>(uint32_t& data);
			ObjectStream& operator>>(int32_t& data);
			ObjectStream& operator>>(uint64_t& data);
			ObjectStream& operator>>(int64_t& data);
			ObjectStream& operator>>(real32_t& data);
			ObjectStream& operator>>(real64_t& data);
			template <typename T>
			ObjectStream& operator>>(std::vector<T>& data)
			{
				uint16_t size;
				*this >> size;
				data.reserve(size);
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::vector<T*>& data)
			{
				uint16_t size;
				*this >> size;
				data.resize(size);
				for (int i = 0; i < size; i++) {
					uint16_t dataI;
					*this >> dataI;
					if (dataI == i) {
						T* elm = new T();
						*this >> *elm;
						data.at(i) = elm;
					}
					else
					{
						data.at(i) = data.at(dataI);
					}
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator>>(std::list<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::list<T*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> elms;
				for (int i = 0; i < size; i++) {
					uint16_t elmI;
					*this >> elmI;
					if (elmI == elms.size())
					{
						T* elm = new T();
						*this >> *elm;
						elms.push_back(elm);
					}
					data.push_back(elms.at(elmI));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::set<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::set<T*>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* elm = new T();
					*this >> *elm;
					data.push_back(elm);
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::multiset<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::multiset<T*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> elms;
				for (int i = 0; i < size; i++) {
					uint16_t elmI;
					*this >> elmI;
					if (elmI == elms.size())
					{
						T* elm = new T();
						*this >> *elm;
						elms.push_back(elm);
					}
					data.push_back(elms.at(elmI));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::unordered_set<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::unordered_set<T*>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* elm = new T();
					*this >> *elm;
					data.push_back(elm);
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::unordered_multiset<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.push_back(std::move(elm));
				}
				return *this;
			}

			template <typename T>
			ObjectStream& operator >> (std::unordered_multiset<T*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> elms;
				for (int i = 0; i < size; i++) {
					uint16_t elmI;
					*this >> elmI;
					if (elmI == elms.size())
					{
						T* elm = new T();
						*this >> *elm;
						elms.push_back(elm);
					}
					data.push_back(elms.at(elmI));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator>>(std::map<T, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T key;
					*this >> key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::map<T, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T key;
					*this >> key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::map<T*, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T* key = new T();
					*this >> *key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::multimap<T, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T key;
					*this >> key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::multimap<T, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T key;
					*this >> key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::multimap<T*, D>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> keys;
				for (int i = 0; i < size; i++)
				{
					uint16_t keyI;
					*this >> keyI;
					T* key = nullptr;
					if (keyI == keys.size()) {
						key = new T();
						*this >> *key;
						keys.push_back(key);
					}
					else
					{
						key = keys.at(keyI);
					}
					D val;
					*this >> val;
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::multimap <T*, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> keys;
				std::vector <D*> vals;
				for (int i = 0; i < size; i++)
				{
					uint16_t keyI;
					*this >> keyI;
					T* key = nullptr;
					if (keyI == keys.size())
					{
						key = new T();
						*this >> *key;
						keys.push_back(key);
					}
					else
					{
						key = keys.at(keyI);
					}
					uint16_t valI;
					*this >> valI;
					D* val = nullptr;
					if (valI == vals.size())
					{
						val = new D();
						*this >> *val;
						vals.push_back(val);
					}
					else
					{
						val = vals.at(valI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_map<T, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T key;
					*this >> key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_map<T, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T key;
					*this >> key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_map<T*, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T* key = new T();
					*this >> *key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_multimap<T, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T key;
					*this >> key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_multimap<T, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <D*> elms;
				for (int i = 0; i < size; i++)
				{
					T key;
					*this >> key;
					uint16_t elmI;
					*this >> elmI;
					D* val = nullptr;
					if (elmI == elms.size())
					{
						val = new D();
						*this >> *val;
						elms.push_back(val);
					}
					else
					{
						val = elms.at(elmI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_multimap<T*, D>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> keys;
				for (int i = 0; i < size; i++)
				{
					uint16_t keyI;
					*this >> keyI;
					T* key = nullptr;
					if (keyI == keys.size()) {
						key = new T();
						*this >> *key;
						keys.push_back(key);
					}
					else
					{
						key = keys.at(keyI);
					}
					D val;
					*this >> val;
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator >> (std::unordered_multimap<T*, D*>& data)
			{
				uint16_t size;
				*this >> size;
				std::vector <T*> keys;
				std::vector <D*> vals;
				for (int i = 0; i < size; i++)
				{
					uint16_t keyI;
					*this >> keyI;
					T* key = nullptr;
					if (keyI == keys.size())
					{
						key = new T();
						*this >> *key;
						keys.push_back(key);
					}
					else
					{
						key = keys.at(keyI);
					}
					uint16_t valI;
					*this >> valI;
					D* val = nullptr;
					if (valI == vals.size())
					{
						val = new D();
						*this >> *val;
						vals.push_back(val);
					}
					else
					{
						val = vals.at(valI);
					}
					data.emplace(std::make_pair(key, val));
				}
				return *this;
			}
		};

		// Defines simplicity for calling objectstream.
#define OBJECTSTREAM comnet::serialization::ObjectStream
#define REF_OBJECTSTREAM comnet::serialization::ObjectStream&
	} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
