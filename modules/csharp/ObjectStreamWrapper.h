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
#pragma once
#include <CommProto/serialization/ObjectStream.h>
#include <HeaderWrapper.h>

using namespace System;
namespace Comnet {
	namespace Serialization{
		public ref class ObjectStreamWrapper sealed{
		public:
			ObjectStream *unmangedObjectStream;
			
			ObjectStreamWrapper();
			~ObjectStreamWrapper();

			int32_t getPosition();
			int32_t getSize();

			void serializeHeader(HeaderWrapper ^ header);
			HeaderWrapper^ deserializeHeader();

			//intput
			void input(String^ data);
			void input(uint8_t& data);
			void input(int8_t& data);
			void input(uint16_t& data);
			void input(int16_t& data);
			void input(int32_t& data);
			void input(uint32_t& data);
			void input(int64_t& data);
			void input(uint64_t& data);
			void input(real32_t& data);
			void input(real64_t& data);

			//output
			void output(String^ data);			
			void output(uint8_t& data);
			void output(int8_t& data);
			void output(uint16_t& data);
			void output(int16_t& data);
			void output(uint32_t& data);
			void output(int32_t& data);
			void output(uint64_t& data);
			void output(int64_t& data);
			void output(real32_t& data);
			void output(real64_t& data);			
		};
	} // Serialization namespace
} // Comnet namespace

