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
#ifndef CCommsLink_H
#define CCommsLink_H


#include <stdint.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <stdlib.h>



#define MAX_CONNECTIONS 32

#pragma managed

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Comnet {
	namespace Network {


		/**
		CommsLink is an Interface that is responsible for the storage of connection addresses.
		Designed in this interface standard, the Connection of the node responsible with this
		adapter-like interface will hold protocols to this specified object.
		*/
		public ref class CommsLink abstract {
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
			virtual Boolean InitConnection( String^ port,  String^ address, uint32_t baudrate) = 0;
			/**
			Add an address to the link.
			*/
			virtual Boolean AddAddress(uint8_t destID,  String^ address, uint16_t port) = 0;
			/**
			Remove an address from the link.
			*/
			virtual Boolean RemoveAddress(uint8_t destID) = 0;
			/**
			Send a message to the specified id of which an address is associated with.
			*/
			virtual Boolean Send(uint8_t destID, uint8_t* txData, uint32_t txLength) = 0;
			/**
			Receive a message from a protocol id.
			*/
			virtual Boolean Recv(uint8_t* rxData, UInt32% rxLength) = 0;
			
		private:
		};
	} // namespace Network
} // namespace Comnet
#endif
