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
	header = new comnet::Header();
}


Header::Header(comnet::Header* header) 
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

void Header::SetDestID(Byte destID){ header->dest_id = destID; }
uint8_t Header::GetDestID(){ return header->dest_id; }

void Header::SetSourceID(Byte sourceID){ header->source_id = sourceID; }
uint8_t Header::GetSourceID(){ return header->source_id; }

void Header::SetMessageLength(UInt16 mesageLength){ header->msg_len = mesageLength; }
uint16_t Header::GetMessageLength(){ return header->msg_len; }

void Header::SetMessageID(UInt16 messageID){ header->msg_id = messageID; }
uint16_t Header::GetMessageID(){ return header->msg_id; }
int32_t Header::GetSourceTime() {
		return header->GetSourceTime();
}

void Header::SetIV(cli::array<Byte>^ IV)
{
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		header->iv[x] = IV[x];
	}
}

cli::array<Byte>^ Header::GetIV()
{
	cli::array<Byte>^IV = gcnew cli::array<Byte>(KEY_LENGTH);
	for (int x = 0; x < KEY_LENGTH; x++)
	{
		IV[x] = header->iv[x];
	}
	return IV;
}