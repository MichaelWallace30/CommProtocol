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
		private ref class IdxPair {
		public:
			IdxPair(UInt16 idx1, UInt16 idx2) 
				:idx1(idx1), idx2(idx2)
			{
			}
			UInt16 idx1;
			UInt16 idx2;
		};
		ref class ObjectStream;
		public ref class ObjSerializable {
		public:
			static UInt64 ObjIDCounter= 0;
			ObjSerializable() {
				id = ObjIDCounter++;
			}
			virtual void Output(ObjectStream^ objStream) = 0;
			virtual void Input(ObjectStream^ objStream) = 0;
			virtual ObjSerializable^ Create() = 0;
			UInt64 id;
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

			void InputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^ list) {
				UInt16 size = list->Count;
				if (size > 0) {
					System::Collections::Generic::List<ObjSerializable^>^ rStack = gcnew System::Collections::Generic::List<ObjSerializable^>();
					System::Collections::Generic::IEnumerator<ObjSerializable^>^ iter = list->GetEnumerator();
					while (iter->MoveNext()) {
						rStack->Add(iter->Current);
					}
					for (int i = rStack->Count - 1; i >= 0; i--) {
						rStack[i]->Input(this);
					}
				}
				Input(size);
			}

			void Input(System::Collections::Generic::ICollection<ObjSerializable^>^ list) {
				UInt16 size = list->Count;
				if (size > 0) {
					System::Collections::Generic::Dictionary<UInt64, UInt16>^ elmIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, uint16_t>();
					System::Collections::Generic::IEnumerator<ObjSerializable^>^ iter = list->GetEnumerator();
					System::Collections::Generic::List<ObjSerializable^>^ rStack = gcnew System::Collections::Generic::List<ObjSerializable^>();
					UInt16 i = 0;
					while (iter->MoveNext()) {
						UInt16 idx;
						if (!elmIndexDict->ContainsKey(iter->Current->id)) {
							elmIndexDict->Add(iter->Current->id, i);
						}
						rStack->Add(iter->Current);
						i++;
					}
					i--;
					while (true) {
						ObjSerializable^ obj = rStack[i];
						UInt16 idx;
						if (elmIndexDict->TryGetValue(obj->id, idx)) {
							if (idx == i) {
								obj->Input(this);
							}
							Input(idx);
						}
						if (i == 0) {
							break;
						}
						i--;
					}
				}
				Input(size);
			}

			void InputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
				UInt16 size = (UInt16)map->Count;
				if (size > 0) {
					System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ iter = map->GetEnumerator();
					while (iter->MoveNext()) {
						iter->Current.Value->Input(this);
						iter->Current.Key->Input(this);
					}
				}
				Input(size);
			}

			void InputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
				UInt16 size = (UInt16)map->Count;
				if (size > 0) {
					System::Collections::Generic::Dictionary<UInt64, IdxPair^>^ valIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, IdxPair^>();
					System::Collections::Generic::List<ObjPair^>^ rStack = gcnew System::Collections::Generic::List<ObjPair^>();
					System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ iter = map->GetEnumerator();
					UInt16 valI = 0;
					UInt16 i = 0;
					while (iter->MoveNext()) {
						if (!valIndexDict->ContainsKey(iter->Current.Value->id)) {
							valIndexDict->Add(iter->Current.Value->id, gcnew IdxPair(i, valI));
							valI++;
						}
						rStack->Add(gcnew ObjPair(iter->Current.Key, iter->Current.Value));
						i++;
					}
					i--;
					while (true) {
						IdxPair^ valIdxPair = valIndexDict[rStack[i]->obj2->id];
						if (valIdxPair->idx1 == i) {
							rStack[i]->obj2->Input(this);
						}
						Input(valIdxPair->idx2);
						rStack[i]->obj1->Input(this);
						if (i == 0) {
							break;
						}
						i--;
					}
				}
				Input(size);
			}
			
			void Input(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
				UInt16 size = map->Count;
				if (size > 0) {
					System::Collections::Generic::Dictionary<UInt64, IdxPair^>^ keyIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, IdxPair^>();
					System::Collections::Generic::Dictionary<UInt64, IdxPair^>^ valIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, IdxPair^>();
					System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ iter = map->GetEnumerator();
					System::Collections::Generic::List<ObjPair^>^ rStack = gcnew System::Collections::Generic::List<ObjPair^>();
					UInt16 keyI = 0;
					UInt16 valI = 0;
					UInt16 i = 0;
					while (iter->MoveNext()) {
						if (!keyIndexDict->ContainsKey(iter->Current.Key->id)) {
							keyIndexDict->Add(iter->Current.Key->id, gcnew IdxPair(i, keyI));
							keyI++;
						}
						if (!valIndexDict->ContainsKey(iter->Current.Value->id)) {
							valIndexDict->Add(iter->Current.Value->id, gcnew IdxPair(i, valI));
							valI++;
						}
						rStack->Add(gcnew ObjPair(iter->Current.Key, iter->Current.Value));
						i++;
					}
					i--;
					while (true) {
						IdxPair^ valIdxPair = valIndexDict[rStack[i]->obj2->id];
						if (valIdxPair->idx1 == i) {
							rStack[i]->obj2->Input(this);
						}
						Input(valIdxPair->idx2);
						IdxPair^ keyIdxPair = keyIndexDict[rStack[i]->obj1->id];
						if (keyIdxPair->idx1 == i) {
							rStack[i]->obj1->Input(this);
						}
						Input(keyIdxPair->idx2);
						if (i == 0) {
							break;
						}
						i--;
					}
				}
				Input(size);
			}

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
			
			void OutputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp) {
				UInt16 size = OutputUInt16();
				for (int i = 0; i < size; i++) {
					ObjSerializable^ obj = temp->Create();
					obj->Output(this);
					list->Add(obj);
				}
			}
			
			void Output(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp) {
				System::Collections::Generic::List<ObjSerializable^>^ data = gcnew System::Collections::Generic::List<ObjSerializable^>();
				UInt16 size;
				size = OutputUInt16();
				for (int i = 0; i < size; i++) {
					UInt16 dataI;
					dataI = OutputUInt16();
					if (dataI == i) {
						ObjSerializable^ obj = temp->Create();
						obj->Output(this);
						list->Add(obj);
						data->Add(obj);
					}
					else
					{
						list->Add(data[dataI]);
						data->Add(data[dataI]);
					}
				}
			}

			void OutputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp)
			{
				UInt16 size = OutputUInt16();
				for (int i = 0; i < size; i++) {
					ObjSerializable^ keyObj = keyTemp->Create();
					keyObj->Output(this);
					ObjSerializable^ valObj = valTemp->Create();
					valObj->Output(this);
					map->Add(System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>(keyObj, valObj));
				}
			}

			void OutputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp)
			{
				UInt16 size = OutputUInt16();
				System::Collections::Generic::List<ObjSerializable^>^ vals = gcnew System::Collections::Generic::List<ObjSerializable^>();
				for (int i = 0; i < size; i++) {
					ObjSerializable^ key = keyTemp->Create();
					key->Output(this);
					UInt16 valI = OutputUInt16();
					ObjSerializable^ val = nullptr;
					if (valI == vals->Count) {
						val = valTemp->Create();
						val->Output(this);
						vals->Add(val);
					}
					else
					{
						val = vals[valI];
					}
					map->Add(System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>(key, val));
				}
			}

			void Output(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp) {
				UInt16 size = OutputUInt16();
				System::Collections::Generic::List<ObjSerializable^>^ keys = gcnew System::Collections::Generic::List<ObjSerializable^>();
				System::Collections::Generic::List<ObjSerializable^>^ vals = gcnew System::Collections::Generic::List<ObjSerializable^>();
				for (int i = 0; i < size; i++) {
					UInt16 keyI = OutputUInt16();
					ObjSerializable^ key = nullptr;
					if (keyI == keys->Count) {
						key = keyTemp->Create();
						key->Output(this);
						keys->Add(key);
					}
					else
					{
						key = keys[keyI];
					}
					UInt16 valI = OutputUInt16();
					ObjSerializable^ val = nullptr;
					if (valI == vals->Count) {
						val = valTemp->Create();
						val->Output(this);
						vals->Add(val);
					}
					else
					{
						val = vals[valI];
					}
					map->Add(System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>(key, val));
				}
			}

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
