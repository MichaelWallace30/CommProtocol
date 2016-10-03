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

#define COMM_PROGRAM_START_TIME __TIME__
#define COMM_PROGRAM_START_DATE __DATE__

#if COM_TARGET_OS == COM_OS_WINDOWS 
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
          c(c& obj) { } \
          c& operator=(c& obj) { }

#endif // __INCLUDE_DEFINES_H
