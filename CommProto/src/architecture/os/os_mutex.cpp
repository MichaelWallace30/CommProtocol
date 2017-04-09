/*
  C style Mutex functions.

  Copyright (C) 2016  Mario Garcia, Michael Wallace.

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
#include <CommProto/architecture/os/os_mutex.h>

#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS
/**
   TODO(): Need to implement Windows mutex functions. 
*/
void mutex_init(mutex_t* mutex) 
{
  *mutex = CreateMutex(NULL, FALSE, NULL);
}

void mutex_lock(mutex_t* mutex)
{
  WaitForSingleObject(*mutex, INFINITE);
}

void mutex_unlock(mutex_t* mutex)
{
  ReleaseMutex(*mutex);
}

void mutex_destroy(mutex_t* mutex)
{
  CloseHandle(*mutex);
}

#else // TARGET_OS_LINUX && TARGET_OS_APPLE

void mutex_init(mutex_t* mutex)
{
  pthread_mutex_init(mutex, NULL);
}

void mutex_lock(mutex_t* mutex)
{
  pthread_mutex_lock(mutex);
}

void mutex_unlock(mutex_t* mutex)
{
  pthread_mutex_unlock(mutex);
}

void mutex_destroy(mutex_t* mutex)
{
  pthread_mutex_destroy(mutex);
}
#endif // COM_TARGET_OS == COM_OS_WINDOWS
