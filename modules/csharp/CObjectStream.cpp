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
using namespace Serialization;

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

Int32 CObjectStream::getPosition()
{
	return unmangedObjectStream->get().getPostion();
}

Int32 CObjectStream::getSize()
{
	return unmangedObjectStream->get().getSize();
}

void CObjectStream::serializeHeader(CHeader ^ header)
{
	unmangedObjectStream->get().headerPacket.destID = header->getDestID();
	unmangedObjectStream->get().headerPacket.sourceID = header->getSourceID();
	unmangedObjectStream->get().headerPacket.messageLength = header->getMessageLength();
	unmangedObjectStream->get().headerPacket.messageID = header->getMessageID();
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		unmangedObjectStream->get().headerPacket.IV[x] = header->getIV()[x];
	}
	unmangedObjectStream->get().serializeHeader(unmangedObjectStream->get().headerPacket);
}

CHeader^ CObjectStream::deserializeHeader()
{
	unmangedObjectStream->get().headerPacket = unmangedObjectStream->get().deserializeHeader();
	CHeader^ header = gcnew CHeader();
	header->setDestID(unmangedObjectStream->get().headerPacket.destID);
	header->setSourceID(unmangedObjectStream->get().headerPacket.sourceID);
	header->setMessageLength(unmangedObjectStream->get().headerPacket.messageLength);
	header->setMessageID(unmangedObjectStream->get().headerPacket.messageID);
	array<Byte>^ IV = gcnew array<Byte>(KEY_LENGTH);
	for(int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = unmangedObjectStream->get().headerPacket.IV[x];
	}
	header->setIV(IV);
	return header;
}

//intput
void CObjectStream::input(String^ data)
 {
	std::string value = msclr::interop::marshal_as<std::string>(data);
	unmangedObjectStream->get() << value;
}
void CObjectStream::input(Byte data)  { unmangedObjectStream->get() << data; }
void CObjectStream::input(SByte data) { unmangedObjectStream->get() << data; }
void CObjectStream::input(UInt16 data){ unmangedObjectStream->get() << data; }
void CObjectStream::input(Int16 data) { unmangedObjectStream->get() << data; }
void CObjectStream::input(Int32 data) { unmangedObjectStream->get() << data; }
void CObjectStream::input(UInt32 data){ unmangedObjectStream->get() << data; }
void CObjectStream::input(Int64 data) { unmangedObjectStream->get() << data; }
void CObjectStream::input(UInt64 data){ unmangedObjectStream->get() << data; }
void CObjectStream::input(Single data){ unmangedObjectStream->get() << data; }
void CObjectStream::input(Double data){ unmangedObjectStream->get() << data; }

//Ooutput
String^ CObjectStream::outputString()
{
	std::string value;
  ObjectStream& obj = unmangedObjectStream->get();
	obj >> value;
	return gcnew String(value.c_str());	
}
Byte CObjectStream::outputByte()    { uint8_t data; unmangedObjectStream->get() >> data; return data; }
SByte CObjectStream::outputSByte()  { int8_t data; unmangedObjectStream->get() >> data; return data; }
UInt16 CObjectStream::outputUInt16(){ UInt16 data; unmangedObjectStream->get() >> data; return data; }
Int16 CObjectStream::outputInt16()  { Int16 data;  unmangedObjectStream->get() >> data; return data; }
UInt32 CObjectStream::outputUInt32(){ UInt32 data; unmangedObjectStream->get() >> data; return data; }
Int32 CObjectStream::outputInt32()  { 
    Int32 data;  
    unmangedObjectStream->get() >> data;
    return data; 
}
UInt64 CObjectStream::outputUInt64(){ UInt64 data; unmangedObjectStream->get() >> data; return data; }
Int64 CObjectStream::outputInt64()  { Int64 data;  unmangedObjectStream->get() >> data; return data; }
Single CObjectStream::outputSingle(){ Single data; unmangedObjectStream->get() >> data; return data; }
Double CObjectStream::outputDouble(){ Double data; unmangedObjectStream->get() >> data; return data; }


