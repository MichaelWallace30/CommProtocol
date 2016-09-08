#ifndef __SERIAL_STATUS_H
#define __SERIAL_STATUS_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

enum serial_status {

  SERIAL_OPEN,
  SERIAL_CLOSED,
  SERIAL_CONNECTED,
  SERIAL_TRANSMIT,

};

#endif // __SERIAL_STATUS_H
