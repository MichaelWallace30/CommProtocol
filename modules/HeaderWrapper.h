#include <CommProto/HeaderPacket.h>
#pragma once
namespace Comnet{
#pragma managed	
public ref class HeaderWrapper sealed
{
	public:
		header_t *header;	
		HeaderWrapper();
		~HeaderWrapper();
		void setDestID(uint8_t destID);
		uint8_t getDestID();
		void setSourceID(uint8_t sourceID);
		uint8_t getSourceID();
		void setMessageLength(uint16_t mesageLength);
		uint16_t getMessageLength();
		void setMessageID(uint16_t messageID);
		uint16_t getMessageID();		
		void setIV(array<uint8_t>^ IV);		
		array<uint8_t>^ getIV();
	};
}
