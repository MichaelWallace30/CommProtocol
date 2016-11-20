/* 
  Serial configurations.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

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

/* Serial message parser implemented
 <start><header><data><checksum><End>*/
#ifndef SERIAL_H
#define SERIAL_H

#include <CommProto/network/parser.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial-config.h> 
#include <CommProto/architecture/macros.h>//str_length(char*, int)
#include <CommProto/debug/comms_debug.h>
#include <CommProto/headerpacket.h>

namespace comnet {
namespace network {

/**
  Serial class
 */
class COMM_EXPORT Serial {
private:
  int32_t id;
  serial_t h_serial;
  //serial private data
  bool connection_established;

  Parser parser;
  /** serial buffer to hold recieved data for parsing messages*/
  uint8_t buffer_recv[MAX_BUFFER_SIZE + (2 * TERMINAL_SEQUENCE_SIZE)];
  uint8_t buffer_send[MAX_BUFFER_SIZE + (2 * TERMINAL_SEQUENCE_SIZE)];

public:

 
  Serial();
  Serial(uint32_t id);
  ~Serial();
  
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
      Returns false if opening comport fails
   */
  bool OpenConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0);
  /**
    Sends txData using its length of bytes through the serial connection. Connection is broadcast 
    dest_id is only used for packing / unpacking. 

    @return false if no proper connection is establish
   */	
  bool Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length);
  /** 
    Sets recieved data to rxData and sets the length of the data to rxLength

    @param rxData
    @param txLength
    @return false if not aviable connection, No data is recieved, or time out
   */
  bool Recv(uint8_t* rx_data, uint32_t* rx_length);

  serial_status GetStatus();

  bool CloseSerialPort();

  serial_t& GetSerialPort();

  void SetId(uint32_t id)
    { this->id = id; }
};
} // namespace Network
} // namespace Comnet
#endif
