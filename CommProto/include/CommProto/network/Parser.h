#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/HeaderPacket.h>

#pragma once
namespace Comnet {
namespace Network {

#define TERMINAL_SEQUENCE_SIZE 3		

class Parser{
	/** terminal sequence for start and stop of message data*/
	char terminal_sequence[TERMINAL_SEQUENCE_SIZE];
	/** Holds current position or parsed data used for reading serial data*/
	uint32_t parserPosition;
	/** last recieveed length for parser information*/
	uint32_t lastReceivedLength;



public:
	Parser();
	~Parser(){};
	
	/** method takes tx buffer and adds terminal to front and end. Also returns adjusted size by reference*/
	void parseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData);
	/** method takes rx buffer and removes terminal to front and end. Also returns adjusted size by reference*/
	bool parseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData);
	/** method determines if buffer has been parsed completely*/
	bool parseReceiveDone(){ return parserPosition == 0 || parserPosition >= lastReceivedLength - 1;}

};


}//end NETWORK namespace
}//end COMNET namespace