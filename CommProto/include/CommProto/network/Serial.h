/* 
  Serial configurations.

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

/* Serial needs a message parser implemented
	<start><header><checksum><data>*/
#ifndef SERIAL_H
#define SERIAL_H

#include <CommProto/network/Parser.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial-config.h> 
#include <CommProto/architecture/macros.h>//str_length(char*, int)
#include <CommProto/debug/CommsDebug.h>
#include <CommProto/HeaderPacket.h>

namespace Comnet {
namespace Network {

/**
  Serial class
 */
class Serial {
private:
  int32_t id;
  serial_t hSerial;
  //serial private data
  bool connectionEstablished;

  Parser parser;
  /** serial buffer to hold recieved data for parsing messages*/
  uint8_t bufferReceive[MAX_BUFFER_SIZE + (2 * TERMINAL_SEQUENCE_SIZE)];
  uint8_t bufferSend[MAX_BUFFER_SIZE + (2 * TERMINAL_SEQUENCE_SIZE)];

public:

	
  Serial();
  Serial(uint32_t id);
  virtual ~Serial();
  
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
      Returns false if opening comport fails
   */
  virtual bool openConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0);
  /**
    Sends txData using its length of bytes through the serial connection. Connection is broadcast 
    destID is only used for packing / unpacking. 

    @return false if no proper connection is establish
   */	
  virtual bool send(uint8_t destID, uint8_t* txData, uint32_t txLength);
  /** 
    Sets recieved data to rxData and sets the length of the data to rxLength

    @param rxData
    @param txLength
    @return false if not aviable connection, No data is recieved, or time out
   */
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength);

  serial_status getStatus();

  bool closePort();

  serial_t& getSerialPort();

  void setId(uint32_t id)
    { this->id = id; }
};
} // namespace Network
} // namespace Comnet
#endif
