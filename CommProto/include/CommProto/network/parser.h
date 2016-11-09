/*
  Parser algorithm.

  Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

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
#include <CommProto/headerpacket.h>

#pragma once
namespace comnet {
namespace network {


class COMM_EXPORT Parser {
 /** terminal sequence for start and Stop of message data*/
 char terminal_sequence[TERMINAL_SEQUENCE_SIZE];
 /** Holds current position or parsed data used for reading serial data*/
 uint32_t parser_position;
 /** last recieveed length for parser information*/
 uint32_t last_received_length;



public:
 Parser();
 ~Parser(){};
 
 /** 
    method takes tx buffer and adds terminal to Front and End. Also returns adjusted size by reference
   */
 void ParseSend(uint8_t* tx_data, uint32_t &tx_length, uint8_t* crc_data, uint8_t *parsed_data);
 /** 
    method takes rx buffer and removes terminal to Front and End. Also returns adjusted size by reference
   */
 bool ParseReceive(uint8_t* rx_data, uint32_t &rx_length, uint8_t *parsed_data);
 /** 
    method determines if buffer has been parsed completely
   */
 bool ParseReceiveDone()
    { return parser_position == 0 || parser_position >= last_received_length - 1;}

};


}//End NETWORK namespace
}//End COMNET namespace
#endif // __PARSER_H
