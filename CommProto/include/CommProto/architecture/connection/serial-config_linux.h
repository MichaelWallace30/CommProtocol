#ifndef __SERIAL_CONFIG_LINUX_H
#define __SERIAL_CONFIG_LINUX_H

#include <CommProto/architecture/connection/serial_status.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


#define ClosePort close



struct serial_info {
  
  speed_t baudrate;
  serial_status serial_s;
  port_id fd;

};

typedef struct serial_info serial_t;
#endif // __SERIAL_CONFIG_LINUX_H
