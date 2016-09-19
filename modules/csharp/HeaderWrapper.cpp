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

void HeaderWrapper::setDestID(Byte destID){ header->destID = destID; }
Byte HeaderWrapper::getDestID(){ return header->destID; }

void HeaderWrapper::setSourceID(Byte sourceID){ header->sourceID = sourceID; }
Byte HeaderWrapper::getSourceID(){ return header->sourceID; }

void HeaderWrapper::setMessageLength(UInt16 mesageLength){ header->messageLength = mesageLength; }
UInt16 HeaderWrapper::getMessageLength(){ return header->messageLength; }

void HeaderWrapper::setMessageID(UInt16 messageID){ header->messageID = messageID; }
UInt16 HeaderWrapper::getMessageID(){ return header->messageID; }

void HeaderWrapper::setIV(array<Byte>^ IV)
{
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		header->IV[x] = IV[x];
	}
}

array<Byte>^ HeaderWrapper::getIV()
{
	array<Byte>^IV = gcnew array<Byte>(KEY_LENGTH);
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = header->IV[x];
	}
	return IV;
}