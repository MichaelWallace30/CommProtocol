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

using namespace System;
using namespace Comnet;
using namespace Comnet::Serialization;

ObjectStream::ObjectStream()
{
	unmangedObjectStream = new CommPointer<comnet::serialization::ObjectStream>(comnet::serialization::ObjectStream());
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
	return unmangedObjectStream->get().getPostion();
}

Int32 ObjectStream::GetSize()
{
	return unmangedObjectStream->get().getSize();
}

void ObjectStream::SerializeHeader(Header ^ header)
{
	unmangedObjectStream->get().headerPacket.destID = header->GetDestID();
	unmangedObjectStream->get().headerPacket.sourceID = header->GetSourceID();
	unmangedObjectStream->get().headerPacket.messageLength = header->GetMessageLength();
	unmangedObjectStream->get().headerPacket.messageID = header->GetMessageID();
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		unmangedObjectStream->get().headerPacket.IV[x] = header->GetIV()[x];
	}
	unmangedObjectStream->get().serializeHeader(unmangedObjectStream->get().headerPacket);
}

Header^ ObjectStream::DeserializeHeader()
{
	unmangedObjectStream->get().headerPacket = unmangedObjectStream->get().deserializeHeader();
	Header^ header = gcnew Header();
	header->SetDestID(unmangedObjectStream->get().headerPacket.destID);
	header->SetSourceID(unmangedObjectStream->get().headerPacket.sourceID);
	header->SetMessageLength(unmangedObjectStream->get().headerPacket.messageLength);
	header->SetMessageID(unmangedObjectStream->get().headerPacket.messageID);
	array<Byte>^ IV = gcnew array<Byte>(KEY_LENGTH);
	for(int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = unmangedObjectStream->get().headerPacket.IV[x];
	}
	header->SetIV(IV);
	return header;
}

//intput
void ObjectStream::Input(String^ data)
 {
	std::string value = msclr::interop::marshal_as<std::string>(data);
	unmangedObjectStream->get() << value;
}
void ObjectStream::Input(Byte data)  { unmangedObjectStream->get() << data; }
void ObjectStream::Input(SByte data) { unmangedObjectStream->get() << data; }
void ObjectStream::Input(UInt16 data){ unmangedObjectStream->get() << data; }
void ObjectStream::Input(Int16 data) { unmangedObjectStream->get() << data; }
void ObjectStream::Input(Int32 data) { unmangedObjectStream->get() << data; }
void ObjectStream::Input(UInt32 data){ unmangedObjectStream->get() << data; }
void ObjectStream::Input(Int64 data) { unmangedObjectStream->get() << data; }
void ObjectStream::Input(UInt64 data){ unmangedObjectStream->get() << data; }
void ObjectStream::Input(Single data){ unmangedObjectStream->get() << data; }
void ObjectStream::Input(Double data){ unmangedObjectStream->get() << data; }

//Ooutput
String^ ObjectStream::OutputString()
{
	std::string value;
  comnet::serialization::ObjectStream& obj = unmangedObjectStream->get();
	obj >> value;
	return gcnew String(value.c_str());	
}
Byte ObjectStream::OutputByte()    { uint8_t data; unmangedObjectStream->get() >> data; return data; }
SByte ObjectStream::OutputSByte()  { int8_t data; unmangedObjectStream->get() >> data; return data; }
UInt16 ObjectStream::OutputUInt16(){ UInt16 data; unmangedObjectStream->get() >> data; return data; }
Int16 ObjectStream::OutputInt16()  { Int16 data;  unmangedObjectStream->get() >> data; return data; }
UInt32 ObjectStream::OutputUInt32(){ UInt32 data; unmangedObjectStream->get() >> data; return data; }
Int32 ObjectStream::OutputInt32()  { 
    Int32 data;  
    unmangedObjectStream->get() >> data;
    return data; 
}
UInt64 ObjectStream::OutputUInt64(){ UInt64 data; unmangedObjectStream->get() >> data; return data; }
Int64 ObjectStream::OutputInt64()  { Int64 data;  unmangedObjectStream->get() >> data; return data; }
Single ObjectStream::OutputSingle(){ Single data; unmangedObjectStream->get() >> data; return data; }
Double ObjectStream::OutputDouble(){ Double data; unmangedObjectStream->get() >> data; return data; }


