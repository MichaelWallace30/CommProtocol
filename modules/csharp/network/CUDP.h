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
#ifndef CUDP_H
#define CUDP_H

/* User Includes */

#define ADDRESS_LENGTH 16
#define MAX_CONNECTIONS 32
#include <network/CCommsLink.h>
#include <CommProto/network/UDP.h>

namespace Comnet {
	namespace Network {
    #pragma managed
		
		public ref class CUDP: public CCommsLink{
		private:			
			UDP *unmanagedUDP;

		public:
			/**Constuctor*/
			CUDP();
			~CUDP();

			/** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
			Returns false if open socket or bind fails*/
			virtual Boolean initConnection( String^ port,  String^ address, uint32_t baudrate)override;
			/** Adds Address & port to destID value of array of aviable connections
			Returns false if connection is already connected*/
			virtual Boolean addAddress(uint8_t destID,  String^ address, uint16_t port)override;
			/** Sets connection to not available
			Returns false is no connection is found*/
			virtual Boolean removeAddress(uint8_t destID)override;
			/** Sends txData using its length of bytes through the destID connection which is establish through add adress
			Return false if no proper connection is establish*/
			virtual Boolean send(uint8_t destID, uint8_t* txData, uint32_t txLength)override;
			/** Sets recieved data to rxData and sets the length of the data to rxLength
			Returns false if not aviable connection or no data is recieved*/
			virtual Boolean recv(uint8_t* rxData, UInt32% rxLength) override;

		};
	} // namespace Network
} // namespace Comnet
#endif
