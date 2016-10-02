/* 
  Mutex Definitions.

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
#ifndef __OS_MUTEX_H
#define __OS_MUTEX_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/os/mutex-config.h>

COM_EXTERN_C
/**
   initialize the mutex.
*/
void mutex_init(mutex_t* mutex);
/**
   Lock this mutex prevent any other thread from accessing the same resource.
*/
void mutex_lock(mutex_t* mutex);
/**
   Unlock the mutex to allow other threads to access the resource.
*/
void mutex_unlock(mutex_t* mutex);
/**
   Destroy the mutex.
*/
void mutex_destroy(mutex_t* mutex);

END_EXTERN_C

#endif // __OS_MUTEX_H
