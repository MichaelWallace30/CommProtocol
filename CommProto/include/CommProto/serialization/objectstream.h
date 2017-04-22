/*
Serial configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

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
#include <CommProto/debug/log.h>
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
			void PrintErrorUnderFlow() { debug::Log::Message(debug::LOG_ERROR, "\nERROR:\t Source: ObjectStream\t Message: Buffer too small\n"); }
			/** Error Handingling methods*/
			void PrintErrorOverFlow() { debug::Log::Message(debug::LOG_ERROR, "\nERROR:\t Source: ObjectStream\t Message: Buffer full\n"); }

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
			ObjectStream& operator<<(const ObjSerializable& data);

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
			ObjectStream& operator>>(ObjSerializable& data);

			/*
			p means pointer compatible.  If it is there that means that it is compatible
			when containing pointers.

			(io) If there is an i then it is only input compatible. If there is an o then it is only
			output compatible.
			Example:
			VECTOR:
			std::set (i)  Vector can be made from set but a set cannot be made from a vector
			*/

			/*
			VECTOR: Compatible with
			std::list (io) p
			std::set (i) [If no duplicate, is o compatible]
			std::unordered_set (i) [If no duplicates, is o compatible]
			std::multiset (io) p
			std::unordered_multiset (io) p
			*/

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
			ObjectStream& operator<<(const std::vector<T*>& data)
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
			ObjectStream& operator >> (std::vector<T*>& data)
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

			/*
			LIST: Compatible with
			std::vector (io) p
			std::set (i) [If no duplicate, is o compatible]
			std::unordered_set (i) [If no duplicates, is o compatible]
			std::multiset (io) p
			std::unordered_multiset (io) p
			*/

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

			/*
			SET: Compatible with
			std::vector (o) [Is i compatible if no duplicates]
			std::list (o) [Is i compatible if no duplicates]
			std::unordered_set (o) p
			std::multiset (o) [Is i compatible if no duplicates]
			std::unordered_multiset (o) [Is i compatible if no duplicates]
			*/

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

			template <typename T>
			ObjectStream& operator >> (std::set<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.emplace(std::move(elm));
				}
				return *this;
			}



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
			ObjectStream& operator >> (std::set<T*>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* elm = new T();
					*this >> *elm;
					data.emplace(elm);
				}
				return *this;
			}

			/*
			MultiSet: Compatible with
			std::vector (io) p
			std::list (io) p
			std::set (i) [If no duplicate, is o compatible]
			std::unordered_set (i) [If no duplicates, is o compatible]
			std::unordered_multiset (io) p
			*/

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

			/*
			UnorderedSet: Compatible with
			std::vector (o)  [Is i compatible if no duplicates]
			std::list (o)  [Is i compatible if no duplicates]
			std::set (io) p
			std::multiset (o) [Is i compatible if no duplicates]
			std::unordered_multiset (o)  [Is i compatible if no duplicates]
			*/

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

			template <typename T>
			ObjectStream& operator >> (std::unordered_set<T>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T elm;
					*this >> elm;
					data.emplace(std::move(elm));
				}
				return *this;
			}



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
			ObjectStream& operator >> (std::unordered_set<T*>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* elm = new T();
					*this >> *elm;
					data.emplace(elm);
				}
				return *this;
			}

			/*
			UnorderedMultiSet: Compatible with
			std::vector (io) p
			std::list (io) p
			std::set (i) [If no duplicate, is o compatible]
			std::unordered_set (i) [If no duplicates, is o compatible]
			std::multiset (io) p
			*/

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
					do {
						i--;
						it--;
						auto mapIter = elmIndexMap.find(*it);
						if (mapIter->second.first == i) {
							*this << *mapIter->first;
						}
						*this << mapIter->second.second;
					} while (it != data.begin());
				}
				*this << size;
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

			/*
			Map: Compatible with
			std::multimap (io)  vp
			std::unordered_map (io)  kp vp kvp
			std::unordered_multimap (io)  vp
			*/

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
			ObjectStream& operator<<(const std::map<T*, D>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++) {
					*this << it->second;
					*this << *it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator>>(std::map<T*, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* key = new T();
					*this >> *key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
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

			/*
			Multimap: Compatible with
			std::map (io)  vp
			std::unordered_map (io)  vp
			std::unordered_multimap (io)  kp vp kvp
			*/

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

			/*
			UnorderedMap: Compatible with
			std::map (io)  kp vp kvp
			std::multimap (io)  vp
			std::unordered_multimap (io)  vp
			*/

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
			ObjectStream& operator<<(const std::unordered_map<T*, D>& data)
			{
				for (auto it = data.begin(); it != data.end(); it++) {
					*this << it->second;
					*this << *it->first;
				}
				uint16_t size = data.size();
				*this << size;
				return *this;
			}

			template <typename T, typename D>
			ObjectStream& operator>>(std::unordered_map<T*, D>& data)
			{
				uint16_t size;
				*this >> size;
				for (int i = 0; i < size; i++) {
					T* key = new T();
					*this >> *key;
					D val;
					*this >> val;
					data.emplace(std::make_pair(std::move(key), std::move(val)));
				}
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
					do {
						i--;
						it--;
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
					} while (it != data.begin());
				}
				*this << size;
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
					do {
						i--;
						it--;
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << *it->first;
					} while (it != data.begin());
				}
				*this << size;
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

			/*
			UnorderedMultiMap: Compatible with
			std::map (io)  vp
			std::multimap (io)  kp vp kvp
			std::unordered_map (io)  vp
			*/

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
					do {
						i--;
						it--;
						auto elmMapIter = elmIndexMap.find(it->second);
						if (elmMapIter->second.first == i) {
							*this << *elmMapIter->first;
						}
						*this << elmMapIter->second.second;
						*this << it->first;
					} while (it != data.begin());
				}
				*this << size;
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
					do {
						i--;
						it--;
						*this << it->second;
						auto keyMapIter = keyIndexMap.find(it->first);
						if (keyMapIter->second.first == i) {
							*this << *keyMapIter->first;
						}
						*this << keyMapIter->second.second;
					} while (it != data.begin());
				}
				*this << size;
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
					do {
						i--;
						it--;
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
					} while (it != data.begin());
				}
				*this << size;
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
