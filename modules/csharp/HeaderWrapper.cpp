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