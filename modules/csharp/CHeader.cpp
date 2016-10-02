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
#include <CHeader.h>

using namespace Comnet;
CHeader::CHeader()
{
	header = new header_t;
}


CHeader::CHeader(header_t* header) 
{
  this->header = header;
}


CHeader::~CHeader()
{
	if (header != NULL)
	{
		delete header;
		header = NULL;
	}
}

void CHeader::SetDestID(Byte destID){ header->destID = destID; }
uint8_t CHeader::GetDestID(){ return header->destID; }

void CHeader::SetSourceID(Byte sourceID){ header->sourceID = sourceID; }
uint8_t CHeader::GetSourceID(){ return header->sourceID; }

void CHeader::SetMessageLength(UInt16 mesageLength){ header->messageLength = mesageLength; }
uint16_t CHeader::GetMessageLength(){ return header->messageLength; }

void CHeader::SetMessageID(UInt16 messageID){ header->messageID = messageID; }
uint16_t CHeader::GetMessageID(){ return header->messageID; }

void CHeader::SetIV(cli::array<Byte>^ IV)
{
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		header->IV[x] = IV[x];
	}
}

cli::array<Byte>^ CHeader::GetIV()
{
	cli::array<Byte>^IV = gcnew cli::array<Byte>(KEY_LENGTH);
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = header->IV[x];
	}
	return IV;
}