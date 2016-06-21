/* 
  Abstract Network configuration.

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
#ifndef CommsLink_H
#define CommsLink_H


#include <stdint.h>

#include <CommProto/CommsPacket.h>

#define MAX_CONNECTIONS 25

class CommsLink
{
public:
  CommsLink(){ ; }
  virtual ~CommsLink(){ ; }
  
  virtual bool initConnection(const char* port = NULL, const char* = NULL, uint32_t baudrate = 0){ return false; }
  virtual bool addAddress(uint8_t destID, const char* address = NULL, uint16_t port = 0){ return false; }
  virtual bool removeAddress(uint8_t destID){ return false; }
  
  virtual bool send(uint8_t destID, uint8_t* txData, int32_t txLength){ return false; }
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength){ return false; }
private:
};

#endif
