/*
Serial configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __OBJECT_STREAM_H
#define __OBJECT_STREAM_H

#include <CommProto/serialization/Marshal.h>
#include <CommProto/debug/CommsDebug.h>
#include <CommProto/HeaderPacket.h>
#include <string>

namespace Comnet {
namespace Serialization {
/** Buffer Limit predefinded */
#define STREAM_BUFFER_MAX_SIZE 1024

	/** Object Stream Class will hold an array of data as a stream. 
		All data will be put into the stream with the overloaded operators '<<' 
		All data will be taken out of the stream with the overloaded operators '>>'
		The Stream is a stack First In First Out. Therefore reverse the order data is put in when taking out.
		To input a char* buffer you can call setBuffer(buffer, length).
		Setting the buffer will erase information by setting current postion to new size of buffer.
		Inputing a buffer is used on the lower level when dealing directly with a data link.
	*/
	class ObjectStream {
	private:
		/** Data stream to hold input as an array of bytes*/
		marshall_t streamBuffer;

		/** Current postion of streamBuffer and also size of streamBuffer*/
		int32_t currentPostion;
		/** Error Handingling methods*/
		void printErrorUnderFlow(){ COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer too small\n"); }
		/** Error Handingling methods*/
		void printErrorOverFlow(){ COMMS_DEBUG("\nERROR:\t Source: ObjectStream\t Message: Buffer full\n"); }

	public:

		/** Header packet for communication*/
		header_t headerPacket;

		/** Constructor*/
		ObjectStream();
		/** Destructor*/
		~ObjectStream();
		/** getPostion returns the postion of the stream buffer*/
		int32_t getPostion(){ return currentPostion; }
		/** getSize return the size of the current buffer*/
		int32_t getSize(){ return currentPostion; }
		/** getBuffer returns a pionter to the buffer stream*/
		marshall_t getBuffer(){return streamBuffer;}
		/** setBuffer will take a pointer to an array of char and its len to become the new stream of data*/
		void setBuffer(const char* buffer, int len);

		
  
		void serializeHeader(header_t header);
		header_t deserializeHeader();

		//
		//decrypt and encrypt header here
		//
		
  
	  /** Overloaded input stream operators to put variables into the object stream*/  
	  ObjectStream& operator<<(string_t& data);
	  ObjectStream& operator<<(std::wstring& data);
	  ObjectStream& operator<<(uint8_t& data);  
	  ObjectStream& operator<<(int8_t& data);
	  ObjectStream& operator<<(uint16_t& data);
	  ObjectStream& operator<<(int16_t& data);  
	  ObjectStream& operator<<(int32_t& data);
	  ObjectStream& operator<<(uint32_t& data);
	  ObjectStream& operator<<(int64_t& data);
	  ObjectStream& operator<<(uint64_t& data);
	  ObjectStream& operator<<(real32_t& data);
	  ObjectStream& operator<<(real64_t& data);
  

	  /** OVerloaded output stream operators to output variables to a variable from the object stream
		  string_t (char*) must use malloc when inputing data into new c-string variable*/  
	  ObjectStream& operator>>(string_t& data);
	  ObjectStream& operator>>(std::wstring& data);
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

};
} // Serialization namespace
} // Comnet namespace

#endif // __OBJECT_STREAM_H
