#include <CommProto/architecture/connection/xbee_serial.h>

#if (COM_TARGET_OS == COM_OS_WINDOWS)

#include <wtypes.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>

#define XBEE_ASSERT(ptr) do { if (xbee_invalid(ptr)) return XBEE_INVALID; } while (0)

xbool xbee_invalid(xbee_serial_t* serial) {
  if (serial) {
    return (xbool )(serial->h_serial == NULL);
  }

  return XBEE_TRUE;
}


xbee_err xbee_open(xbee_serial_t* serial, speed_t baudrate) {
  char buffer[sizeof("\\\\.\\COM0000")];
  HANDLE h_com;
  COMMTIMEOUTS timeouts;
  int err;
  
  if (serial == NULL || serial->fd > 9999) {
    return XBEE_INVALID;
  }

  if (serial->h_serial) {
    h_com = serial->h_serial;
    serial->h_serial = NULL;
#if 0
    printf("%s: port already open (h_com=%p)\n", FUNCTOR_NAME, h_com);
#endif 
  } else {
    snprintf(buffer, sizeof buffer, "\\\\.\\COM%u", serial->h_serial);
    h_com = CreateFile(buffer, GENERIC_READ | GENERIC_WRITE,  0, NULL, 
                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (h_com == INVALID_HANDLE_VALUE) {
      return XBEE_IO;
    } 
  }

  SetupComm(h_com, XBEE_RX_BUFFER_LENGTH, XBEE_TX_BUFFER_LENGTH);
 
  // Setup our new serial comms timeouts.

  if (!GetCommTimeouts(h_com, &timeouts)) {
    ClosePort( h_com );
    return XBEE_IO;
  }
  timeouts.ReadIntervalTimeout = MAXDWORD;
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant  = 0;

  if (!SetCommTimeouts(h_com, &timeouts)) {
    ClosePort(h_com);
    return XBEE_IO;
  }

  PurgeComm(h_com, PURGE_TXCLEAR | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_RXABORT);

  serial->h_serial = h_com;
  err = xbee_change_baudrate(serial, baudrate);
  if (err) {
    serial->h_serial = NULL;
    return (xbee_err )err;  
  }

  // Xbee serial is now open!
  return XBEE_SUCCESS;
}


xbee_err xbee_close(xbee_serial_t* serial) {
  XBEE_ASSERT(serial);
  
  ClosePort(serial->h_serial);
  serial->h_serial = NULL;

  return XBEE_SUCCESS;
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS