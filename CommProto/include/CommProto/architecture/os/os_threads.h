/* 
  Thread definitions.

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
#ifndef OSTHREADS_H 
#define OSTHREADS_H 
 
 
#include <CommProto/architecture/os/include_defines.h> 
#include <CommProto/architecture/os/thread-config.h> 

 
COM_EXTERN_C  
 
 
//posix \ window threading 
  /* Aside from the function pointer taking void* as argument and returning void*, nothing special here  
 */ 
void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg); 

 
thread_t thread_get_self_id(); 

 
#if COMMPROTO_TARGET_OS != COMMMPROTO_OS_WINDOWS 
unsigned Sleep(unsigned seconds); 
#endif // COM_TARGET_OS != COM_OS_WINDOWS 

 

 
END_EXTERN_C 

 
#endif//OSTHREAD_H 
