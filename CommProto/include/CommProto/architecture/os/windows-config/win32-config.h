/* 
  Windows specific configuration.

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
#ifndef __WIN32_CONFIG_H
#define __WIN32_CONFIG_H

#include <CommProto/architecture/os/arch.h>


#if (COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS)

#include <assert.h>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#define FUNCTOR_NAME __FUNCTION__

#if (COM_DISABLE_ASSERT > 0)
 #define COM_ASSERT(cond)
#else
 #define COM_ASSERT(cond)             assert(cond)
#endif
#define COM_UNUSED_PARAM(unusedparam) (void)unusedparam


/* Define NULL pointer value */
#ifndef NULL
 #ifdef __cplusplus
  #define NULL 0
 #else
  #define NULL ((void*)0)
 #endif
#endif

#endif // COM_TARGET_OS == COM_OS_WINDOWS 
#endif // __WIN32_CONFIG_H_
