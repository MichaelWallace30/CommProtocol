/*
 Include Defines available for the operating system build.
 */
#ifndef __INCLUDE_DEFINES_H
#define __INCLUDE_DEFINES_H

#include <CommProtocol/architecture/os/arch.h>

#if COM_TARGET_OS == COM_OS_WINDOWS
 #include <CommProtocol/architecture/os/windows-config/win32-config.h>
 #include <stdint.h>
#elif COM_TARGET_OS == COM_OS_LINUX
 #include <CommProtocol/architecture/os/linux-config/linux-config.h>
#elif COM_TARGET_OS == COM_OS_APPLE
 #include <CommProtocol/architecture/os/mac-config/mac-config.h>
#else
 #error "Target OS is unknown. OS may be incompatible with this software."
#endif // COM_TARGET_OS == COM_OS_WINDOWS

#endif // __INCLUDE_DEFINES_H
