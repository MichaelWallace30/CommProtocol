#include <HeaderWrapper.h>

using namespace Comnet;
HeaderWrapper::HeaderWrapper()
{
	header = new header_t();
}

HeaderWrapper::~HeaderWrapper()
{
	if (header != NULL)
	{
		delete header;
		header = NULL;
	}
}

void HeaderWrapper::setDestID(uint8_t destID){ header->destID = destID; }
uint8_t HeaderWrapper::getDestID(){ return header->destID; }

void HeaderWrapper::setSourceID(uint8_t sourceID){ header->sourceID = sourceID; }
uint8_t HeaderWrapper::getSourceID(){ return header->sourceID; }

void HeaderWrapper::setMessageLength(uint16_t mesageLength){ header->messageLength = mesageLength; }
uint16_t HeaderWrapper::getMessageLength(){ return header->messageLength; }

void HeaderWrapper::setMessageID(uint16_t messageID){ header->messageID = messageID; }
uint16_t HeaderWrapper::getMessageID(){ return header->messageID; }

void HeaderWrapper::setIV(array<uint8_t>^ IV)
{
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		header->IV[x] = IV[x];
	}
}

array<uint8_t>^ HeaderWrapper::getIV()
{
	array<uint8_t>^IV = gcnew array<uint8_t>(KEY_LENGTH);
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = header->IV[x];
	}
	return IV;
}