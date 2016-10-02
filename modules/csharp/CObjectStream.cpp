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
#include <CObjectStream.h>
#include <msclr/marshal_cppstd.h>
#include <string>

using namespace System;
using namespace Comnet;
using namespace Comnet::Serialization;
using namespace comnet;
using namespace comnet::tools::allocator;
using namespace comnet::serialization;

CObjectStream::CObjectStream()
{
	unmangedObjectStream = new CommPointer<ObjectStream>(ObjectStream());
}

CObjectStream::CObjectStream(CommPointer<ObjectStream>* pointer) 
: unmangedObjectStream(new CommPointer<ObjectStream>(*pointer))
{
}

CObjectStream::~CObjectStream()
{
	delete unmangedObjectStream;
	unmangedObjectStream = NULL;
}

Int32 CObjectStream::GetPosition()
{
	return unmangedObjectStream->get().getPostion();
}

Int32 CObjectStream::GetSize()
{
	return unmangedObjectStream->get().getSize();
}

void CObjectStream::SerializeHeader(CHeader ^ header)
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

CHeader^ CObjectStream::DeserializeHeader()
{
	unmangedObjectStream->get().headerPacket = unmangedObjectStream->get().deserializeHeader();
	CHeader^ header = gcnew CHeader();
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
void CObjectStream::Input(String^ data)
 {
	std::string value = msclr::interop::marshal_as<std::string>(data);
	unmangedObjectStream->get() << value;
}
void CObjectStream::Input(Byte data)  { unmangedObjectStream->get() << data; }
void CObjectStream::Input(SByte data) { unmangedObjectStream->get() << data; }
void CObjectStream::Input(UInt16 data){ unmangedObjectStream->get() << data; }
void CObjectStream::Input(Int16 data) { unmangedObjectStream->get() << data; }
void CObjectStream::Input(Int32 data) { unmangedObjectStream->get() << data; }
void CObjectStream::Input(UInt32 data){ unmangedObjectStream->get() << data; }
void CObjectStream::Input(Int64 data) { unmangedObjectStream->get() << data; }
void CObjectStream::Input(UInt64 data){ unmangedObjectStream->get() << data; }
void CObjectStream::Input(Single data){ unmangedObjectStream->get() << data; }
void CObjectStream::Input(Double data){ unmangedObjectStream->get() << data; }

//Ooutput
String^ CObjectStream::OutputString()
{
	std::string value;
  ObjectStream& obj = unmangedObjectStream->get();
	obj >> value;
	return gcnew String(value.c_str());	
}
Byte CObjectStream::OutputByte()    { uint8_t data; unmangedObjectStream->get() >> data; return data; }
SByte CObjectStream::OutputSByte()  { int8_t data; unmangedObjectStream->get() >> data; return data; }
UInt16 CObjectStream::OutputUInt16(){ UInt16 data; unmangedObjectStream->get() >> data; return data; }
Int16 CObjectStream::OutputInt16()  { Int16 data;  unmangedObjectStream->get() >> data; return data; }
UInt32 CObjectStream::OutputUInt32(){ UInt32 data; unmangedObjectStream->get() >> data; return data; }
Int32 CObjectStream::OutputInt32()  { 
    Int32 data;  
    unmangedObjectStream->get() >> data;
    return data; 
}
UInt64 CObjectStream::OutputUInt64(){ UInt64 data; unmangedObjectStream->get() >> data; return data; }
Int64 CObjectStream::OutputInt64()  { Int64 data;  unmangedObjectStream->get() >> data; return data; }
Single CObjectStream::OutputSingle(){ Single data; unmangedObjectStream->get() >> data; return data; }
Double CObjectStream::OutputDouble(){ Double data; unmangedObjectStream->get() >> data; return data; }


