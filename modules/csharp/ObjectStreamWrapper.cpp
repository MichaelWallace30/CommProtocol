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
#include <ObjectStreamWrapper.h>
#include <msclr/marshal_cppstd.h>
#include <string>

using namespace System;
using namespace Comnet;
using namespace Serialization;

ObjectStreamWrapper::ObjectStreamWrapper()
{
	unmangedObjectStream = new ObjectStream();
}

ObjectStreamWrapper::~ObjectStreamWrapper()
{
	delete unmangedObjectStream;
	unmangedObjectStream = NULL;
}

Int32 ObjectStreamWrapper::getPosition()
{
	return unmangedObjectStream->getPostion();
}

Int32 ObjectStreamWrapper::getSize()
{
	return unmangedObjectStream->getSize();
}

void ObjectStreamWrapper::serializeHeader(HeaderWrapper ^ header)
{
	unmangedObjectStream->headerPacket.destID = header->getDestID();
	unmangedObjectStream->headerPacket.sourceID = header->getSourceID();
	unmangedObjectStream->headerPacket.messageLength = header->getMessageLength();
	unmangedObjectStream->headerPacket.messageID = header->getMessageID();
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		unmangedObjectStream->headerPacket.IV[x] = header->getIV()[x];
	}
	unmangedObjectStream->serializeHeader(unmangedObjectStream->headerPacket);
}

HeaderWrapper^ ObjectStreamWrapper::deserializeHeader()
{
	unmangedObjectStream->headerPacket = unmangedObjectStream->deserializeHeader();
	HeaderWrapper^ header = gcnew HeaderWrapper();
	header->setDestID(unmangedObjectStream->headerPacket.destID);
	header->setSourceID(unmangedObjectStream->headerPacket.sourceID);
	header->setMessageLength(unmangedObjectStream->headerPacket.messageLength);
	header->setMessageID(unmangedObjectStream->headerPacket.messageID);
	array<Byte>^ IV = gcnew array<Byte>(KEY_LENGTH);
	for(int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = unmangedObjectStream->headerPacket.IV[x];
	}
	header->setIV(IV);
	return header;
}

//intput
void ObjectStreamWrapper::input(String^ data)
 {
	std::string value = msclr::interop::marshal_as<std::string>(data);
	*unmangedObjectStream << value;
}
void ObjectStreamWrapper::input(Byte data)  { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(SByte data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(UInt16 data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(Int16 data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(Int32 data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(UInt32 data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(Int64 data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(UInt64 data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(Single data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(Double data){ *unmangedObjectStream << data; }

//Ooutput
String^ ObjectStreamWrapper::outputString()
{
	std::string value;
	*unmangedObjectStream >> value;
	return gcnew String(value.c_str());	
}
Byte ObjectStreamWrapper::outputByte()    { uint8_t data;*unmangedObjectStream >> data; return data; }
SByte ObjectStreamWrapper::outputSByte()  { int8_t data; *unmangedObjectStream >> data; return data; }
UInt16 ObjectStreamWrapper::outputUInt16(){ UInt16 data; *unmangedObjectStream >> data; return data; }
Int16 ObjectStreamWrapper::outputInt16()  { Int16 data;  *unmangedObjectStream >> data; return data; }
UInt32 ObjectStreamWrapper::outputUInt32(){ UInt32 data; *unmangedObjectStream >> data; return data; }
Int32 ObjectStreamWrapper::outputInt32()  { Int32 data;  *unmangedObjectStream >> data; return data; }
UInt64 ObjectStreamWrapper::outputUInt64(){ UInt64 data; *unmangedObjectStream >> data; return data; }
Int64 ObjectStreamWrapper::outputInt64()  { Int64 data;  *unmangedObjectStream >> data; return data; }
Single ObjectStreamWrapper::outputSingle(){ Single data; *unmangedObjectStream >> data; return data; }
Double ObjectStreamWrapper::outputDouble(){ Double data; *unmangedObjectStream >> data; return data; }


