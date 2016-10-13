/*
UDP configurations.

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
#ifndef CUDPLink_H
#define CUDPLink_H

/* User Includes */

#include <network/CommsLink.h>
#include <CommProto/network/UDPLink.h>


namespace Comnet {
  namespace Network {
    #pragma managed		


    public ref class UDPLink : public CommsLink {
    private:			
      comnet::network::UDPLink *unmanagedUDPLink;

    public:
      /**Constuctor*/
      UDPLink();
      ~UDPLink();

			/** 
        Opens socket, assigns local address & port, binds socket, sets slen 
        to length of address, sets is connected on success.
			  
        @returns false if open socket or bind fails need baudrate for interface 
                of CCommsLink method is overloaded below.
       */
      Boolean InitConnection( String^ port,  String^ address, uint32_t baudrate) override;
			/** 
         Overloaded  initConnection
       */
      Boolean InitConnection(String^ port, String^ address);
			/** 
        Adds Address & port to destID value of array of aviable connections
			  @returns false if connection is already connected
       */
      Boolean AddAddress(uint8_t destID,  String^ address, uint16_t port) override;
			/** 
        Sets connection to not available
			  @returns false is no connection is found
       */
      Boolean RemoveAddress(uint8_t destID) override;
			/** 
        Sends txData using its length of bytes through the destID connection which 
        is establish through add address.
			  
        @return false if no proper connection is establish
       */
      Boolean Send(uint8_t destID, uint8_t* txData, uint32_t txLength) override;
			/** 
        Sets recieved data to rxData and sets the length of the data to rxLength
			  @returns false if not aviable connection or no data is recieved
       */
      Boolean Recv(uint8_t* rxData, UInt32% rxLength) override;
    };
  } // namespace Network
} // namespace Comnet
#endif
