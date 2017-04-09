/* 
  Defined Contributions.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 Include Defines available for the operating system build.
 */
#ifndef __INCLUDE_DEFINES_H
#define __INCLUDE_DEFINES_H


#include <CommProto/architecture/os/arch.h>
#include <stdint.h>


typedef float real32_t;
typedef double real64_t;


#define COMM_PROGRAM_START_TIME __TIME__
#define COMM_PROGRAM_START_DATE __DATE__

#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS 
 #include <CommProto/architecture/os/windows-config/win32-config.h>
#elif COM_TARGET_OS == COM_OS_LINUX
 #include <CommProto/architecture/os/linux-config/linux-config.h>
#elif COM_TARGET_OS == COM_OS_APPLE
 #include <CommProto/architecture/os/mac-config/mac-config.h>
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


#define COMM_DISALLOW_COPYING(c) \
          c(c& obj) = delete; \
          c& operator=(c& obj) = delete; 

#define NOEXCEPT noexcept
#define THREAD_LOCAL thread_local

 /*
 TODO(): This will need to be defined with windows.
 */
#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS
 #define FORCE_INLINE __forceinline 
 #if defined(_MSC_VER) && defined(NOEXCEPT)
  #if _MSC_VER <= 1800
   #undef NOEXCEPT
   #define NOEXCEPT
   #undef THREAD_LOCAL
   #define THREAD_LOCAL
  #endif
 #endif
#else
 #define FORCE_INLINE __inline __attribute__((always_inline))
#endif // COM_TARGET_OS == COM_OS_WINDOWS

#endif // __INCLUDE_DEFINES_H
