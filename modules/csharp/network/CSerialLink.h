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
#include <network/CCommsLink.h>
#include <CommProto/network/SerialLink.h>

#pragma managed
using namespace System;

namespace Comnet {
namespace Network {
/**
  CSerial class for managed c++
 */
public ref class CSerial:public CCommsLink{
private:
  SerialLink *unmanagedSerialLink;

  
public:	
  CSerial(); 
  virtual ~CSerial();
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
  Returns false if opening comport fails*/
  virtual Boolean initConnection(String^ port, String^ address, uint32_t baudrate)override;
  /** OVerride initConncetion)*/
  virtual Boolean initConnection(String^ port, uint32_t baudrate);
  /** Add serial address returns true does nothing */
  virtual Boolean addAddress(uint8_t destID, String^ address, uint16_t port)override;
  /** Remove serial address returns true does nothing */
  virtual Boolean removeAddress(uint8_t destID)override;
  /**
  Sends txData using its length of bytes through the serial connection. Connection is broadcast
  destID is only used for packing / unpacking. Return false if no proper connection is establish
  */
  virtual Boolean send(uint8_t destID, uint8_t* txData, uint32_t txLength)override;
  /** Sets recieved data to rxData and sets the length of the data to rxLength
  Returns false if not aviable connection, No data is recieved, or time out*/
  virtual bool recv(uint8_t* rxData, uint32_t% rxLength)override;

};
} // namespace Network
} // namespace Comnet
#endif
