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

#define XBEE_MS_TIMER_RESOLUTION     16


// Hex dump declarations.
#define HEX_DUMP_NONE                0x0000

#define HEX_DUMP_OFFSET              0x0001

#define HEX_DUMP_ADDR                0x0002

#define HEX_DUMP_TAB                 0x0004


// Delta time for zigbee
#define ZIGBEE_TIME_DELTA_1970       ((UINT32_C(30) * 365 + 7) * 24 * 60 * 60)


/*
  xbee error enumerator. Used in xbee/zigbee/digimesh functions.
  This will grow as we continue through the functionality of the XBee protocol.
 */
enum xbee_errors {
  XBEE_SUCCESS                          =    0,
  XBEE_FAILED                           =   -1,
  XBEE_INVALID                          =   -2,
  XBEE_IO                               =   -3,
  XBEE_UNKNOWN                          =   -4,
  
};

typedef enum xbee_errors xbee_err;
typedef int32_t bool_t;

#define XBEE_TRUE                    1
#define XBEE_FALSE                   0

COM_EXTERN_C


xbee_err xbee_open(xbee_serial_t* serial, speed_t baudrate);
xbee_err xbee_close(xbee_serial_t* serial);

xbee_err xbee_write(xbee_serial_t* serial, const void* buffer, uint32_t length);
xbee_err xbee_read(xbee_serial_t* serial, void* buffer, uint32_t length);

const char* xbee_get_portname(xbee_serial_t* serial);
bool_t xbee_invalid(xbee_serial_t* serial);

int32_t xbee_free_tx(xbee_serial_t* serial);
int32_t xbee_used_tx(xbee_serial_t* serial);
int32_t xbee_flush_tx(xbee_serial_t* serial);

int32_t xbee_free_rx(xbee_serial_t* serial);
int32_t xbee_used_rx(xbee_serial_t* serial);
int32_t xbee_flush_rx(xbee_serial_t* serial);

int32_t xbee_change_baudrate(xbee_serial_t* serial);
int32_t xbee_break(xbee_serial_t* serial, bool_t enabled);
int32_t xbee_flowcontrol(xbee_serial_t* serial, bool_t enabled);

int32_t xbee_set_rts(xbee_serial_t* serial, bool_t asserted);
int32_t xbee_get_cts(xbee_serial_t* serial);


uint32_t xbee_seconds(void);
uint32_t xbee_milliseconds(void);

int xbee_hex_str_to_bytes(const char* ptr);
int xbee_readline(char* buffer, int32_t length);

void hex_dump(const void* addr, uint16_t length, uint16_t flags);

END_EXTERN_C

#endif // __XBEE_SERIAL_H
