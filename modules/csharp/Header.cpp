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
#include <Header.h>

using namespace Comnet;
Header::Header()
{
	header = new header_t;
}


Header::Header(header_t* header) 
{
  this->header = header;
}


Header::~Header()
{
	if (header != NULL)
	{
		delete header;
		header = NULL;
	}
}

void Header::SetDestID(Byte destID){ header->destID = destID; }
uint8_t Header::GetDestID(){ return header->destID; }

void Header::SetSourceID(Byte sourceID){ header->sourceID = sourceID; }
uint8_t Header::GetSourceID(){ return header->sourceID; }

void Header::SetMessageLength(UInt16 mesageLength){ header->messageLength = mesageLength; }
uint16_t Header::GetMessageLength(){ return header->messageLength; }

void Header::SetMessageID(UInt16 messageID){ header->messageID = messageID; }
uint16_t Header::GetMessageID(){ return header->messageID; }

void Header::SetIV(cli::array<Byte>^ IV)
{
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		header->IV[x] = IV[x];
	}
}

cli::array<Byte>^ Header::GetIV()
{
	cli::array<Byte>^IV = gcnew cli::array<Byte>(KEY_LENGTH);
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = header->IV[x];
	}
	return IV;
}