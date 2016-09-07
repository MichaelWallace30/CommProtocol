#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/HeaderPacket.h>
#include <cstdio>

#pragma once
namespace Comnet {
namespace Network {

#define TERMINAL_SEQUENCE_SIZE 3		

class Parser{
	/** terminal sequence for start and stop of message data*/
	char terminal_sequence[TERMINAL_SEQUENCE_SIZE];




public:
	Parser();
	~Parser(){};
	/** Holds current position or parsed data used for reading serial data*/
	uint32_t parserPosition;
	/** last recieveed length for parser information*/
	uint32_t lastReceivedLength;
	void parseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData);
	bool parseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData);
	bool parseReceiveDone(){ return parserPosition == 0 || parserPosition >= lastReceivedLength - 1;}

};


}//end NETWORK namespace
}//end COMNET namespace