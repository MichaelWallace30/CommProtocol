/*
XBee Link that handles all xbee connections.

Copyright (C) 2016  Mario Garcia.

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
#include <CommProto/network/xbee.h>
//#include <CommProto/network/znodetable.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/headerpacket.h>

#include <regex>
#include <cstring>
#include <ctype.h>




namespace comnet {
namespace network {
	

XBee::XBee(const char* port, speed_t baudrate) 
{
 
}


XBee::~XBee() 
{
	
}


// Initialize the xbee device.
bool XBee::Initialize(const char* port, speed_t baudrate) {
	return false;
}


// Absolutely not sure this will work...
// Need to test this.
bool XBee::Recv(uint8_t* rxData, uint32_t* rxLength) {
	return false;
}


bool XBee::Send(const char* wpan_addr, uint8_t* txData, uint32_t txLength) {
	return false;
}


} // namespace Network
} // namespace Comnet
