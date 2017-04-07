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
#include <ObjectStream.h>
#include <msclr/marshal_cppstd.h>
#include <string>
#include "ObjSerializable.h"

using namespace System;
using namespace Comnet;
using namespace Comnet::Serialization;

ObjectStream::ObjectStream()
{
  try {
   unmangedObjectStream = new CommPointer<comnet::serialization::ObjectStream>(new comnet::serialization::ObjectStream());
  } catch (System::Runtime::InteropServices::SEHException()) {
    Console::WriteLine("SEH ERROR");
  }
}

ObjectStream::ObjectStream(CommPointer<comnet::serialization::ObjectStream>* pointer) 
: unmangedObjectStream(new CommPointer<comnet::serialization::ObjectStream>(*pointer))
{
}

ObjectStream::~ObjectStream()
{
 delete unmangedObjectStream;
 unmangedObjectStream = NULL;
}

Int32 ObjectStream::GetPosition()
{
 return unmangedObjectStream->Get().GetPosition();
}

Int32 ObjectStream::GetSize()
{
 return unmangedObjectStream->Get().GetSize();
}

void ObjectStream::SetHeader(Header^ header)
{
  unmangedObjectStream->Get().header_packet.dest_id = header->GetDestID();
  unmangedObjectStream->Get().header_packet.source_id = header->GetSourceID();
  unmangedObjectStream->Get().header_packet.msg_len = header->GetMessageLength();
  unmangedObjectStream->Get().header_packet.msg_id = header->GetMessageID();
  for (int x = 0; x < KEY_LENGTH; x++)
  {
    unmangedObjectStream->Get().header_packet.iv[x] = header->GetIV()[x];
  }
}

void ObjectStream::SerializeHeader()
{
 unmangedObjectStream->Get().SerializeHeader();
}

Header^ ObjectStream::DeserializeHeader()
{
 unmangedObjectStream->Get().header_packet = unmangedObjectStream->Get().DeserializeHeader();
 Header^ header = gcnew Header();
 header->SetDestID(unmangedObjectStream->Get().header_packet.dest_id);
 header->SetSourceID(unmangedObjectStream->Get().header_packet.source_id);
 header->SetMessageLength(unmangedObjectStream->Get().header_packet.msg_len);
 header->SetMessageID(unmangedObjectStream->Get().header_packet.msg_id);
 array<Byte>^ IV = gcnew array<Byte>(KEY_LENGTH);
 for(int x = 0; x < KEY_LENGTH; x++)
 {
  IV[x] = unmangedObjectStream->Get().header_packet.iv[x];
 }
 header->SetIV(IV);
 return header;
}

void ObjectStream::Input(String^ data)
 {
 std::string value = msclr::interop::marshal_as<std::string>(data);
 unmangedObjectStream->Get() << value;
}
void ObjectStream::Input(Byte data)  { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(SByte data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt16 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int16 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int32 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt32 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Int64 data) { unmangedObjectStream->Get() << data; }
void ObjectStream::Input(UInt64 data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Single data){ unmangedObjectStream->Get() << data; }
void ObjectStream::Input(Double data){ unmangedObjectStream->Get() << data; }

void ObjectStream::Output(String^% data)
{
 std::string value;
  comnet::serialization::ObjectStream& obj = unmangedObjectStream->Get();
 obj >> value;
 data = gcnew String(value.c_str());	
}
void ObjectStream::Output(Byte% data) { uint8_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(SByte% data) { int8_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(UInt16% data){ uint16_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(Int16% data)  { int16_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(UInt32% data){ uint32_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(Int32% data) { int32_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(UInt64% data) { uint64_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(Int64% data) { int64_t temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(Single% data) { float temp; unmangedObjectStream->Get() >> temp; data = temp; }
void ObjectStream::Output(Double% data) { double temp; unmangedObjectStream->Get() >> temp; data = temp; }

//Output
String^ ObjectStream::OutputString()
{
	std::string value;
	comnet::serialization::ObjectStream& obj = unmangedObjectStream->Get();
	obj >> value;
	return gcnew String(value.c_str());
}
Byte ObjectStream::OutputByte() { uint8_t data; unmangedObjectStream->Get() >> data; return data; }
SByte ObjectStream::OutputSByte() { int8_t data; unmangedObjectStream->Get() >> data; return data; }
UInt16 ObjectStream::OutputUInt16() { UInt16 data; unmangedObjectStream->Get() >> data; return data; }
Int16 ObjectStream::OutputInt16() { Int16 data;  unmangedObjectStream->Get() >> data; return data; }
UInt32 ObjectStream::OutputUInt32() { UInt32 data; unmangedObjectStream->Get() >> data; return data; }
Int32 ObjectStream::OutputInt32() {
	Int32 data;
	unmangedObjectStream->Get() >> data;
	return data;
}
UInt64 ObjectStream::OutputUInt64() { UInt64 data; unmangedObjectStream->Get() >> data; return data; }
Int64 ObjectStream::OutputInt64() { Int64 data;  unmangedObjectStream->Get() >> data; return data; }
Single ObjectStream::OutputSingle() { Single data; unmangedObjectStream->Get() >> data; return data; }
Double ObjectStream::OutputDouble() { Double data; unmangedObjectStream->Get() >> data; return data; }

void ObjectStream::InputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^ list) {
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

void ObjectStream::OutputUnique(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp) {
	UInt16 size = OutputUInt16();
	for (int i = 0; i < size; i++) {
		ObjSerializable^ obj = temp->Create();
		obj->Output(this);
		list->Add(obj);
	}
}

void ObjectStream::Input(System::Collections::Generic::ICollection<ObjSerializable^>^ list) {
	UInt16 size = list->Count;
	if (size > 0) {
		System::Collections::Generic::Dictionary<UInt64, IdxPair^>^ elmIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, IdxPair^>();
		System::Collections::Generic::IEnumerator<ObjSerializable^>^ iter = list->GetEnumerator();
		System::Collections::Generic::List<ObjSerializable^>^ rStack = gcnew System::Collections::Generic::List<ObjSerializable^>();
		UInt16 i = 0;
		UInt16 elmI = 0;
		while (iter->MoveNext()) {
			if (!elmIndexDict->ContainsKey(iter->Current->id)) {
				elmIndexDict->Add(iter->Current->id, gcnew IdxPair(i, elmI));
				elmI++;
			}
			rStack->Add(iter->Current);
			i++;
		}
		do {
			i--;
			ObjSerializable^ obj = rStack[i];
			IdxPair^ idxPair;
			if (elmIndexDict->TryGetValue(obj->id, idxPair)) {
				if (idxPair->idx1 == i) {
					obj->Input(this);
				}
				Input(idxPair->idx2);
			}
		} while (i > 0);
	}
	Input(size);
}

void ObjectStream::Output(System::Collections::Generic::ICollection<ObjSerializable^>^% list, ObjSerializable^ temp) {
	System::Collections::Generic::List<ObjSerializable^>^ data = gcnew System::Collections::Generic::List<ObjSerializable^>();
	UInt16 size;
	size = OutputUInt16();
	for (int i = 0; i < size; i++) {
		UInt16 dataI;
		dataI = OutputUInt16();
		if (dataI == data->Count) {
			ObjSerializable^ obj = temp->Create();
			obj->Output(this);
			data->Add(obj);
		}
		list->Add((*data)[dataI]);
	}
}

void ObjectStream::InputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
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

void ObjectStream::OutputUniqueKeyVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp)
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

void ObjectStream::InputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
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
		do {
			i--;
			IdxPair^ valIdxPair = valIndexDict[rStack[i]->obj2->id];
			if (valIdxPair->idx1 == i) {
				rStack[i]->obj2->Input(this);
			}
			Input(valIdxPair->idx2);
			rStack[i]->obj1->Input(this);
		} while (i > 0);
	}
	Input(size);
}

void ObjectStream::OutputUniqueKey(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp)
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

void ObjectStream::InputUniqueVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map)
{
	UInt16 size = (UInt16)map->Count;
	if (size > 0) {
		System::Collections::Generic::Dictionary<UInt64, IdxPair^>^ keyIndexDict = gcnew System::Collections::Generic::Dictionary<UInt64, IdxPair^>();
		System::Collections::Generic::List<ObjPair^>^ rStack = gcnew System::Collections::Generic::List<ObjPair^>();
		System::Collections::Generic::IEnumerator<System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ iter = map->GetEnumerator();
		UInt16 keyI = 0;
		UInt16 i = 0;
		while (iter->MoveNext()) {
			if (!keyIndexDict->ContainsKey(iter->Current.Key->id)) {
				keyIndexDict->Add(iter->Current.Key->id, gcnew IdxPair(i, keyI));
				keyI++;
			}
			rStack->Add(gcnew ObjPair(iter->Current.Key, iter->Current.Value));
			i++;
		}
		do {
			i--;
			rStack[i]->obj2->Input(this);
			IdxPair^ keyIdxPair = keyIndexDict[rStack[i]->obj1->id];
			if (keyIdxPair->idx1 == i) {
				rStack[i]->obj1->Input(this);
			}
			Input(keyIdxPair->idx2);
		} while (i > 0);
	}
	Input(size);
}


void ObjectStream::OutputUniqueVal(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp)
{
	UInt16 size = OutputUInt16();
	System::Collections::Generic::List<ObjSerializable^>^ keys = gcnew System::Collections::Generic::List<ObjSerializable^>();
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
		ObjSerializable^ val = valTemp->Create();
		val->Output(this);
		map->Add(System::Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>(key, val));
	}
}

void ObjectStream::Input(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^ map) {
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
		do {
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
		} while (i > 0);
	}
	Input(size);
}

void ObjectStream::Output(System::Collections::Generic::ICollection<Collections::Generic::KeyValuePair<ObjSerializable^, ObjSerializable^>>^% map, ObjSerializable^ keyTemp, ObjSerializable^ valTemp) {
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
