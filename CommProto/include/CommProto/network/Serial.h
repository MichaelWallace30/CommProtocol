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

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial-config.h> 
#include <CommProto/network/CommsLink.h>
#include <CommProto/architecture/macros.h>//str_length(char*, int)
#include <stdio.h>//printf

#define SERIAL_DEBUG

namespace Comnet {
namespace Network {


class Serial : public CommsLink {
private:
  serial_t hSerial;
  //serial private data
  bool connectionEstablished;

public:

	
  Serial();
  virtual ~Serial();
  
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
      Returns false if opening comport fails*/
  virtual bool initConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0);
  /** Add serial address returns true does nothing */
  virtual bool addAddress(uint8_t destID, const char* address = NULL, uint16_t port = 0) { return true; }
  /** Remove serial address returns true does nothing */
  virtual bool removeAddress(uint8_t destID) { return true; }
  /**
     Sends txData using its length of bytes through the serial connection. Connection is broadcast 
     destID is only used for packing / unpacking. Return false if no proper connection is establish
  */	
  virtual bool send(uint8_t destID, uint8_t* txData, int32_t txLength);
  /** Sets recieved data to rxData and sets the length of the data to rxLength
      Returns false if not aviable connection, No data is recieved, or time out*/
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength);

  serial_status getStatus();

  bool closePort();

  serial_t& getSerialPort();
};
} // namespace Network
} // namespace Comnet
#endif
