/*
  Parser algorithm.

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
#ifndef __PARSER_H
#define __PARSER_H
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/HeaderPacket.h>

#pragma once
namespace comnet {
namespace network {


class Parser {
	/** terminal sequence for start and stop of message data*/
	char terminal_sequence[TERMINAL_SEQUENCE_SIZE];
	/** Holds current position or parsed data used for reading serial data*/
	uint32_t parserPosition;
	/** last recieveed length for parser information*/
	uint32_t lastReceivedLength;



public:
	Parser();
	~Parser(){};
	
	/** 
    method takes tx buffer and adds terminal to front and end. Also returns adjusted size by reference
   */
	void parseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData);
	/** 
    method takes rx buffer and removes terminal to front and end. Also returns adjusted size by reference
   */
	bool parseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData);
	/** 
    method determines if buffer has been parsed completely
   */
	bool parseReceiveDone()
    { return parserPosition == 0 || parserPosition >= lastReceivedLength - 1;}

};


}//end NETWORK namespace
}//end COMNET namespace
#endif // __PARSER_H
