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
