/* 
  Mutex configuration.

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
#ifndef __MUTEX_CONFIG_H
#define __MUTEX_CONFIG_H

#include <CommProto/architecture/os/arch.h>

#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS
 #include <Windows.h>
 typedef HANDLE mutex_t;
#else
 #include <pthread.h>
 #include <unistd.h>
 typedef pthread_mutex_t mutex_t;
#endif // COM_TARGET_OS == COM_OS_WINDOWS
#endif // __MUTEX_CONFIG_H
