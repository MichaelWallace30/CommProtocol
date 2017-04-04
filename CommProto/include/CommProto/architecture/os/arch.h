/* 
  Operating system configuration.

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
/**
   Operating System Architecture Check.
 */
#ifndef __ARCH_H
#define __ARCH_H

// define the target architecture to determine which operating system will be compiled.
#define COMMPROTO_OS_UNKNOWN        0
#define COMMMPROTO_OS_WINDOWS       1
#define COMMPROTO_OS_LINUX          2
#define COMMPROTO_OS_APPLE          3
#define COMMPROTO_OS_QX             4
#define COMMPROTO_OS_BSD            5

// determine our target platform that is going to be used for the enviroment.
#define COMMPROTO_TARGET_OS         COMMPROTO_OS_UNKNOWN

// WIN32 operating systems: Windows.

#if defined( _WIN32)
 #undef COMMPROTO_TARGET_OS
 #define COMMPROTO_TARGET_OS        COMMMPROTO_OS_WINDOWS
#endif // _WIN32 && WINDOWS

// Linux
#if defined(__linux__) && !defined(__APPLE__)
 #undef COMMPROTO_TARGET_OS
 #define COMMPROTO_TARGET_OS        COMMPROTO_OS_LINUX
#endif // __linux__

// Apple : OSX
#if defined(__APPLE__) && defined(__MACH__)
 #undef COMMPROTO_TARGET_OS
 #define COMMPROTO_TARGET_OS        COMMPROTO_OS_APPLE
#endif // __APPLE__ && __MACH__


//---------------------------------------------------
// configuring the target platform.
//---------------------------------------------------
#if !(COMMPROTO_TARGET_OS)
 #error "Operating system is not known. Is it possible you are targeting an unknown platform?"
#endif

// Allow for specializations in what ever operating system we are building in.

#endif // __ARCH_H
