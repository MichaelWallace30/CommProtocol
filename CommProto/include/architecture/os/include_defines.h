/*
 Include Defines available for the operating system build.
 */
#ifndef __INCLUDE_DEFINES_H
#define __INCLUDE_DEFINES_H

#include <CommProto/include/architecture/os/arch.h>
#include <stdint.h>

#if COM_TARGET_OS == COM_OS_WINDOWS
 #include <CommProto/include/architecture/os/windows-config/win32-config.h>
#elif COM_TARGET_OS == COM_OS_LINUX
 #include <CommProto/include/architecture/os/linux-config/linux-config.h>
#elif COM_TARGET_OS == COM_OS_APPLE
 #include <CommProto/include/architecture/os/mac-config/mac-config.h>
#else
 #error "Target OS is unknown. OS may be incompatible with this software."
#endif // COM_TARGET_OS == COM_OS_WINDOWS

#ifdef __cplusplus
 #define COM_EXTERN_C extern "C" {
 #define END_EXTERN_C }               
#else 
 #define COM_EXTERN_C
 #define END_EXTERN_C 
#endif

#endif // __INCLUDE_DEFINES_H
