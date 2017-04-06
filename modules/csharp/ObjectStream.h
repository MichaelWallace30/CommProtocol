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
#include <iostream>

using namespace System;
using namespace System::Collections;
using namespace comnet::tools::allocator;

namespace Comnet {
	namespace Serialization {
		ref class ObjSerializable;

		private ref class IdxPair {
		public:
			IdxPair(UInt16 idx1, UInt16 idx2) 
				:idx1(idx1), idx2(idx2)
			{
			}
			UInt16 idx1;
			UInt16 idx2;
		};
		private ref class ObjPair {
		public:
			ObjPair(ObjSerializable^ obj1, ObjSerializable^ obj2)
				:obj1(obj1), obj2(obj2)
			{
			}
			ObjSerializable^ obj1;
			ObjSerializable^ obj2;
		};

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

			void InputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^ list);

			void Input(System::Collections::Generic::ICollection<ObjSerializable^>^ list);

			void InputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map);

			void InputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map);

			void InputUniqueVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map);
			
			void Input(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map);

			//output
			void Output(String^% data);
			void Output(Byte% data);
			void Output(SByte% data);
			void Output(UInt16% data);
			void Output(Int16% data);
			void Output(UInt32% data);
			void Output(Int32% data);
			void Output(UInt64% data);
			void Output(Int64% data);
			void Output(Single% data);
			void Output(Double% data);
			
			void OutputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp);
			
			void Output(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp);

			void OutputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp);

			void OutputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp);

			void OutputUniqueVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp);

			void Output(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp);

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
