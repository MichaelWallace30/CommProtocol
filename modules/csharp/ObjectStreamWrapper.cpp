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

int32_t ObjectStreamWrapper::getPosition()
{
	return unmangedObjectStream->getPostion();
}

int32_t ObjectStreamWrapper::getSize()
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
	array<uint8_t>^ IV = gcnew array<uint8_t>(KEY_LENGTH);
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
void ObjectStreamWrapper::input(uint8_t& data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(int8_t& data)  { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(uint16_t& data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(int16_t& data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(int32_t& data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(uint32_t& data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(int64_t& data) { *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(uint64_t& data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(real32_t& data){ *unmangedObjectStream << data; }
void ObjectStreamWrapper::input(real64_t& data){ *unmangedObjectStream << data; }

//Ooutput
void ObjectStreamWrapper::output(String^ data)
{
	std::string value;
	*unmangedObjectStream >> value;
	data = gcnew String(value.c_str());
}
void ObjectStreamWrapper::output(uint8_t& data) { *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(int8_t& data)  { *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(uint16_t& data){ *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(int16_t& data) { *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(uint32_t& data){ *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(int32_t& data) { *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(uint64_t& data){ *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(int64_t& data) { *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(real32_t& data){ *unmangedObjectStream >> data; }
void ObjectStreamWrapper::output(real64_t& data){ *unmangedObjectStream >> data; }


