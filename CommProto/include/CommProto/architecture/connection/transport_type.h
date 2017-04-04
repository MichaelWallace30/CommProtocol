/*
Transport protocols in use with the library.

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
#ifndef __TRANSPORT_TYPE_H
#define __TRANSPORT_TYPE_H


/**
  Transport protocol types define the current protocols that are 
  in use right now (with the exception of ZigBee for now). They will be defined for the 
  CommNode children in order to provide proper state machine functionality.
 */
typedef enum transport_protocol_t {
  UDP_LINK,
  SERIAL_LINK,
  ZIGBEE_LINK,
		TCP_LINK
} protocol_t;

#endif // __DATALINK_TYPE_H
