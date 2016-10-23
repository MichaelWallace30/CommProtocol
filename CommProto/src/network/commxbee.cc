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
#include <CommProto/network/commxbee.h>
//#include <CommProto/network/znodetable.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/headerpacket.h>

#include <regex>
#include <cstring>
#include <ctype.h>
#include <xbee.h>

namespace comnet {
namespace network {

CommXBee::CommXBee()
{

}


CommXBee::~CommXBee()
{
	
}


// Initialize the xbee device.
bool CommXBee::Initialize(const char* port, speed_t baudrate) {

	char port_name[55];
	#if (COM_TARGET_OS == COM_OS_WINDOWS)
		sprintf(port_name, "\\\\.\\%s", port);//change to com
	#else
		sprintf(port_name, "%s", port);
	#endif	
	
	if ((ret = xbee_setup(&xbee, "xbee5", port_name, baudrate)) != XBEE_ENONE) {		
		//printf("Construct ret: %d (%s)\n", ret, xbee_errorToStr(ret));		
		return false;
	}	
	return true;
}


// Absolutely not sure this will work...
// Need to test this.
bool CommXBee::Recv(xbee_con *con, uint8_t* rxData, uint32_t* rxLength) {
	
		xbee_conRx(con, &pkt, NULL);	
		// Package is not null, means we got a package from somewhere.
		if (pkt != NULL) {
			for (int i = 0; i < pkt->dataLen; i++) {
				rxData[i] = pkt->data[i];
			}

			*rxLength = pkt->dataLen;
			if (xbee_pktFree(pkt) != XBEE_ENONE);
			return true;
		}		
	return false;
}


bool CommXBee::Send(xbee_con *con, uint8_t* txData, uint32_t txLength) {
		xbee_connTx(con, NULL, txData, txLength);
	return false;
}

} // namespace Network
} // namespace Comnet