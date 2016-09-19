/*
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

			Int32 getPosition();
			Int32 getSize();

			void serializeHeader(HeaderWrapper ^ header);
			HeaderWrapper^ deserializeHeader();

			//intput
			void input(String^ data);
			void input(Byte data);
			void input(SByte data);
			void input(UInt16 data);
			void input(Int16 data);
			void input(Int32 data);
			void input(UInt32 data);
			void input(Int64 data);
			void input(UInt64 data);
			void input(Single data);
			void input(Double data);

			//output
			String^ outputString();			
			Byte outputByte();
			SByte outputSByte();
			UInt16 outputUInt16();
			Int16 outputInt16();
			UInt32 outputUInt32();
			Int32 outputInt32();
			UInt64 outputUInt64();
			Int64 outputInt64();
			Single outputSingle();
			Double outputDouble();			
		};
	} // Serialization namespace
} // Comnet namespace

