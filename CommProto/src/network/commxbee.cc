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


/**
Adds an address to the link.
*/
bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0)
{
	return false;
}
/**
Remove an address from the link.
*/
bool RemoveAddress(uint8_t dest_id)
{
	return false;
}
/**
Send data over to the destination node.
*/

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


uint8_t CommXBee::hexCharToInt(unsigned char value)
{
	if (isalpha(value))
	{
		unsigned char c = toupper(value);
		return c = c - '7';// or c -= 55
	}
	else if (isdigit(value))
	{
		return value = value - '0';
	}
	return 0;
}

// 16 ^ 1 * 15 + 16 ^ 0 * 15
uint8_t CommXBee::doubleHexCharToInt(unsigned char c1, unsigned char c2)
{
	//16 ^ 1 * C1 + 16 ^ 0 * C2
	uint8_t value = hexCharToInt(c1) * 16;
	value += hexCharToInt(c2);
	return value;
}

void CommXBee::stringToAddress(const char* str, uint8_t length, xbee_conAddress &address)
{
	//if no spaces in string increment is 2 if spaces increment is 3
	int increment = (str[2] == ' ') ? 3 : 2;
	address.addr64_enabled = 1;
	int x = 0;
	//check if string is right size to calc double hex char to int
	if ((increment == 2 && (length) % increment == 0)
		|| (increment == 3 && ((length - 1) % increment == 0)))
	{
		//loop for each pair of chars to find int value ex: FF = 255
		for (int index = 0; index < length; index += increment)
		{

			address.addr64[x] = doubleHexCharToInt(str[index], str[index + 1]);
			x++;
		}
	}
	else
	{
		//throw error::InternalException(error::OSErrors::error_no_os, error::error_invalid_hex_string);
		return;
	}
}

} // namespace Network
} // namespace Comnet