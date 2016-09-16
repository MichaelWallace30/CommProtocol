#include <CommProto/architecture/connection/xbee_serial.h>

#if COM_TARGET_OS == COM_OS_LINUX
// serial connections.
#include <stdio.h>
#include <stdlib.h>

// Serial connections specifically for xbee.
#include <limits.h>
#include <fcntl.h>
#include <termios.h>
#include <unitstd.h>
#include <sys/ioctl.h>

#define XBEE_ASSERT(ptr) do { if (xbee_invalid(ptr)) return XBEE_INVALID; } while (0)


xbool xbee_invalid(xbee_serial_t* serial) {
  if (serial && serial->fd >= 0) {
    return XBEE_FALSE;
  }

#if 0
  if (serial == NULL) {
    printf("Serial is null.\n");
  } else {
    printf("xbee_invalid(): serial=%p, serial->fd=%d (invalid)\n", serial, serial->fd);
  }
#endif
 
  return XBEE_TRUE;
}


xbee_err xbee_open(xbee_serial_t* serial, speed_t baudrate) {
  int err;
  
  if (serial == NULL) {
    return XBEE_INVALID;
  }

  serial->device[(sizeof serial->device) - 1] = '\0';

  serial->fd = open(serial->device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (serial->fd < 0) {
    return -errno;
  }

  fcntl(serial->fd, F_SETFL, FNDELAY);
  xbee_change_baudrate(serial, 0);
  err = xbee_change_baudrate(serial, baudrate);

  if (err) {
    return (xbee_err )err;
  }
  
  return XBEE_SUCCESS;
}



xbee_err xbee_close(xbee_serial_t* serial) {
  int result = 0;
  XBEE_ASSERT(serial);
  
  if (close(serial->fd) == -1) {
    result = -errno;
  }

  serial->fd = -1;

  return (xbee_err )result;
}
#endif // COM_TARGET_OS == COM_OS_LINUX
