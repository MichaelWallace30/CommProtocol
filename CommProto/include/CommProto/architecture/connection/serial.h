#ifndef __SERIAL_H
#define __SERIAL_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#if COM_TARGET_OS == COM_OS_WINDOWS 
 #include <Windows.h>

 typedef uint32_t speed_t;
 typedef HANDLE serial_h;
 typedef int32_t port_id;
#else
 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <errno.h>
 #include <termios.h>

 typedef int32_t port_id;
#endif // COM_TARGET_OS == COM_OS_WINDOWS

enum serial_status {
  SERIAL_OPEN,
  SERIAL_CLOSED,
  SERIAL_CONNECTED,
  SERIAL_TRANSMIT,
};


struct serial_info {
  speed_t baudrate;
  serial_status serial_s;

  port_id fd;

#if COM_TARGET_OS == COM_OS_WINDOWS
  serial_h h_serial;
#endif
};

typedef struct serial_info serial_t;

#endif // __SERIAL_H
