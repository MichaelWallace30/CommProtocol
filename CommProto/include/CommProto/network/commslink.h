/* 
  Abstract Network configuration.

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
#ifndef CommsLink_H
#define CommsLink_H


#include <stdint.h>

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/transport_type.h>
#include <CommProto/architecture/api.h>
#include <CommProto/headerpacket.h>

#define MAX_CONNECTIONS 32

namespace comnet {
namespace network {


/**
  CommsLink is an Interface that is responsible for the storage of connection addresses.
  Designed in this interface standard, the Connection of the node responsible with this 
  adapter-like interface will hold protocols to this specified object.
 */
_COMNET_INTERFACE_ class COMM_EXPORT CommsLink {
public:
  /**
    Default Constuctor.
  */  
  CommsLink()
    { }
  /**
    Polymorphic Destructor.
  */  
  virtual ~CommsLink()
    { }
  /**
    Initialize the connection.
  */
  virtual bool InitConnection(const char* port = NULL, const char* = NULL, uint32_t baudrate = 0) = 0;
  /**
    Add an address to the link.
  */  
  virtual bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0) = 0;
  /**
    Remove an address from the link.
  */  
  virtual bool RemoveAddress(uint8_t dest_id) = 0;
  /**
    Send a message to the specified id of which an address is associated with.
  */
  virtual bool Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length) = 0;
  /**
    Receive a message from a protocol id.
  */
  virtual bool Recv(uint8_t* rx_data, uint32_t* rx_length) = 0;

  /**
    Additional commands are used for set configuration features on the protocol of choice.
  */
  virtual void DigestCommand(const char* cmd) = 0;
  
  transport_protocol_t GetProtocol() { return protocol; }
protected:
  // The Protocol type.
  transport_protocol_t protocol;
};
} // namespace Network
} // namespace Comnet
#endif
