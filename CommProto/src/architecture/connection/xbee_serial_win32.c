#include <CommProto/architecture/connection/xbee_serial.h>

#if (COM_TARGET_OS == COM_OS_WINDOWS)

#include <wtypes.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XBEE_ASSERT(ptr) do { if (xbee_invalid(ptr)) return XBEE_INVALID; } while (0)

bool_t xbee_invalid(xbee_serial_t* serial) {
  if (serial) {
    return (serial->h_serial == NULL);
  }

  return XBEE_TRUE;
}


int32_t xbee_change_baudrate(xbee_serial_t* serial , speed_t baudrate) {
  DCB dcb;
  
  if (serial == NULL || serial->h_serial == NULL) {
    return XBEE_INVALID;
  }

  memset(&dcb, 0, sizeof(dcb));
  dcb.DCBlength = sizeof(dcb);
  if (!GetCommState(serial->h_serial, &dcb)) {
    return XBEE_IO;
  }
  dcb.BaudRate = baudrate;
  dcb.ByteSize = 8;
  dcb.Parity = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  dcb.fAbortOnError = FALSE;

  if (!SetCommState(serial->h_serial, &dcb)) {
    return XBEE_IO;
  }

  serial->baudrate = baudrate;
  return XBEE_SUCCESS;
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
    _snprintf(buffer, sizeof buffer, "\\\\.\\COM%u", serial->h_serial);
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


int32_t xbee_write(xbee_serial_t* serial, const void* buffer, uint32_t length) {
  DWORD dwWrote;
  BOOL success;
  
  XBEE_ASSERT(serial);

  if (!buffer || length < 0) {
    return XBEE_INVALID;
  }

  success = WriteFile(serial->h_serial, buffer, length, &dwWrote, NULL);

  if (!success) {
    return XBEE_IO;
  }

  return (int32_t ) dwWrote;
}


int32_t xbee_read(xbee_serial_t* serial, void* buffer, uint32_t length) {
  DWORD dwRead;
  BOOL success;

  XBEE_ASSERT(serial);

  if (!buffer || length < 0) {
    return XBEE_IO;
  }
  
  success = ReadFile(serial->h_serial, buffer, length, &dwRead, NULL);
  
  if (!success) {
    return XBEE_IO;
  }

  return (int32_t ) dwRead;
}


const char* xbee_get_portname(xbee_serial_t* serial) {
  static char portname[sizeof("COM0000")];

  if (serial != NULL && serial->fd > 0 && serial->fd < 10000) {
    _snprintf(portname , sizeof(portname), "COM%u", serial->fd);
    return portname;
  }
  
  return "invalid port";
}


int32_t xbee_used_tx(xbee_serial_t* serial) {
  COMSTAT comstat;

  XBEE_ASSERT(serial);

  if (ClearCommError(serial->h_serial, NULL, &comstat)) {
    return (int32_t ) comstat.cbOutQue;
  }

  return 0;
}


int32_t xbee_flush_tx(xbee_serial_t* serial) {
  XBEE_ASSERT(serial);
  
  PurgeComm(serial->h_serial, PURGE_TXCLEAR | PURGE_TXABORT);

  return 0;
}


int32_t xbee_free_tx(xbee_serial_t* serial) {
  XBEE_ASSERT(serial);

  return XBEE_TX_BUFFER_LENGTH - xbee_used_tx(serial);
}


int32_t xbee_used_rx(xbee_serial_t* serial) {
  COMSTAT comstat;
  
  XBEE_ASSERT(serial);

  if (ClearCommError(serial->h_serial, NULL, &comstat)) {
    return (int32_t ) comstat.cbInQue;
  }

  return 0;
}


int32_t xbee_flush_rx(xbee_serial_t* serial) {
  XBEE_ASSERT(serial);

  PurgeComm(serial->h_serial, PURGE_RXCLEAR | PURGE_RXABORT);

  return 0;
}


int32_t xbee_free_rx(xbee_serial_t* serial) {
  XBEE_ASSERT(serial);

  return XBEE_RX_BUFFER_LENGTH - xbee_used_rx(serial);
}


#endif // COM_TARGET_OS == COM_OS_WINDOWS