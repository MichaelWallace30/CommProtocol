/*
Serial Configuration.

Copyright (C) 2016  Mario Garcia.

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
#ifndef __SERIAL_H
#define __SERIAL_H


#include <CommProto/architecture/os/arch.h>
#include <errno.h>


#if defined(COMMPROTO_TARGET_OS)
 #if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS
  #include <CommProto/architecture/connection/serial-config_win32.h>
 #else
  #include <CommProto/architecture/connection/serial-config_linux.h>
 #endif // COM_TARGET_OS == COM_OS_WINDOWS
#endif // COM_TARGET_OS

#endif // __SERIAL_H
