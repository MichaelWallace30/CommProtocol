#ifndef __SERIAL_H
#define __SERIAL_H


#include <CommProto/architecture/os/arch.h>

#if defined(COM_TARGET_OS)
 #if COM_TARGET_OS == COM_OS_WINDOWS
  #include <CommProto/architecture/connection/serial-config_win32.h>
 #else
  #include <CommProto/architecture/connection/serial-config_linux.h>
 #endif // COM_TARGET_OS == COM_OS_WINDOWS
#endif // COM_TARGET_OS

#endif // __SERIAL_H
