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
#include <CommProto/serialization/objectstream.h>
#include <CommProto/tools/allocator/commpointer.h>
#include "Header.h"

using namespace System;
using namespace comnet::tools::allocator;

namespace Comnet {
	namespace Serialization {
		public ref class ObjectStream sealed {
		public:
			comnet::tools::allocator::CommPointer<comnet::serialization::ObjectStream>* unmangedObjectStream;
			
			ObjectStream();
			~ObjectStream();

			Int32 GetPosition();
			Int32 GetSize();

			void SetHeader(Header^ header);

			void SerializeHeader();
			Header^ DeserializeHeader();

			//intput
			void Input(String^ data);
			void Input(Byte data);
			void Input(SByte data);
			void Input(UInt16 data);
			void Input(Int16 data);
			void Input(Int32 data);
			void Input(UInt32 data);
			void Input(Int64 data);
			void Input(UInt64 data);
			void Input(Single data);
			void Input(Double data);

			//output
			String^ OutputString();			
			Byte OutputByte();
			SByte OutputSByte();
			UInt16 OutputUInt16();
			Int16 OutputInt16();
			UInt32 OutputUInt32();
			Int32 OutputInt32();
			UInt64 OutputUInt64();
			Int64 OutputInt64();
			Single OutputSingle();
			Double OutputDouble();

internal:
  ObjectStream(CommPointer<comnet::serialization::ObjectStream>* pointer);
		};
	} // Serialization namespace
} // Comnet namespace

