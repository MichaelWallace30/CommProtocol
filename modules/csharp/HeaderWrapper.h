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

#include <CommProto/HeaderPacket.h>
using namespace System;
#pragma once
namespace Comnet{
#pragma managed	
public ref class HeaderWrapper sealed
{
	public:
		header_t *header;	
		HeaderWrapper();
		~HeaderWrapper();
		void setDestID(Byte destID);
		uint8_t getDestID();
		void setSourceID(Byte sourceID);
		uint8_t getSourceID();
		void setMessageLength(UInt16 mesageLength);
		uint16_t getMessageLength();
		void setMessageID(UInt16 messageID);
		uint16_t getMessageID();		
		void setIV(array<Byte>^ IV);		
		array<Byte>^ getIV();
	};
}
