#ifndef __SERIAL_CONFIG_WIN32_H
#define __SERIAL_CONFIG_WIN32_H


#include <CommProto/architecture/connection/serial_status.h>
#include <Windows.h>
 

typedef uint32_t speed_t;
typedef HANDLE serial_h;
typedef int32_t port_id;


#define ClosePort CloseHandle


struct serial_info {

  speed_t baudrate;
  serial_status serial_s;
  port_id fd;
  serial_h h_serial;

};

typedef struct serial_info serial_t;

#endif // __SERIAL_CONFIG_WIN32_H
