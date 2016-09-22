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
  SerialLink *unmanagedSerial;

  
public:	
  CSerial(); 
  CSerial(uint32_t id);
  virtual ~CSerial();
  /** Opens comport sets if it is connected on scucces, address is not need just use "" argument
      Returns false if opening comport fails
  */
  virtual Boolean initConnection(String^ port, String^ address, uint32_t baudrate)override;
  /** Overlaoded openConnection*/
  virtual Boolean initConnection(String^ port, uint32_t baudrate);
  /**
    Sends txData using its length of bytes through the serial connection. Connection is broadcast 
    destID is only used for packing / unpacking.

    @return false if no proper connection is establish
  */	
  virtual Boolean send(uint8_t destID, uint8_t* txData, uint32_t txLength)override;

};
} // namespace Network
} // namespace Comnet
#endif
