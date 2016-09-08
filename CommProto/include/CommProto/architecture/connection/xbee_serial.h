/*
  List Interface for CommProtocol.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __XBEE_SERIAL_H
#define __XBEE_SERIAL_H


#include <CommProto/architecture/connection/serial-config.h>

/*
  Cheating, of course, but no point in redefining a new struct, when we can just use
  the same one with an xbee label.
 */
typedef serial_t xbee_serial_t;


#define XBEE_RX_BUFFER_LENGTH        4096

#define XBEE_TX_BUFFER_LENGTH        512

#define XBEE_MS_TIMER_RESOLUTION 16

/*
  xbee error enumerator. Used in xbee/zigbee/digimesh functions.
  This will grow as we continue through the functionality of the XBee protocol.
 */
enum xbee_err {
  XBEE_SUCCESS                          = 0,
  XBEE_UNKNOWN                          = -1,
};
#endif // __XBEE_SERIAL_H
