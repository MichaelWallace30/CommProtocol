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

#ifndef CSERIAL_H
#define CSERIAL_H
#include <network/CommsLink.h>
#include <CommProto/network/seriallink.h>

#pragma managed
using namespace System;

namespace Comnet {
namespace Network {
/**
  CSerial class for managed c++
 */
public ref class SerialLink : public CommsLink {
private:
  comnet::network::SerialLink *unmanagedSerialLink;

  
public:	
  SerialLink();
  ~SerialLink();
  /** 
    Opens comport sets if it is connected on scucces, address is not need just use "" argument
    @returns false if opening comport fails
   */
  Boolean InitConnection(String^ port, String^ address, uint32_t baudrate) override;
  /** 
    OVerride initConncetion.
   */
  Boolean InitConnection(String^ port, uint32_t baudrate);
  /** 
    Add serial address returns true does nothing 
   */
  Boolean AddAddress(uint8_t destID, String^ address, uint16_t port) override;
  /** 
    Remove serial address returns true does nothing 
   */
  Boolean RemoveAddress(uint8_t destID) override;
  /**
    Sends txData using its length of bytes through the serial connection. Connection is broadcast
    destID is only used for packing / unpacking. Return false if no proper connection is establish
  */
  Boolean Send(uint8_t destID, uint8_t* txData, uint32_t txLength) override;
  /** 
    Sets recieved data to rxData and sets the length of the data to rxLength
    @returns false if not aviable connection, No data is recieved, or time out
   */
  bool Recv(uint8_t* rxData, uint32_t% rxLength) override;

};
} // namespace Network
} // namespace Comnet
#endif
