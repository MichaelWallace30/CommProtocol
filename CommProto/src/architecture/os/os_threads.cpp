/*
  C style Thread functions.

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
#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/debug/log.h>

/*
  Define the target OS. Windows and Unix like systems use different APIs.
*/
#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS

/**
   Windows definition to Create a thread.
*/
void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) 
{
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
}

/**
   Get the id of the current thread.
*/
thread_t thread_get_self_id() 
{
  return GetCurrentThread();
}

#else

#include <time.h>
/**
   Create a pthread.
*/
void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) 
{
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, "Starting pthread...\n");
  pthread_create(thread, NULL, start_routine, arg);
}

/**
   Get the current thread id.
*/
thread_t thread_get_self_id() 
{
  return pthread_self();
}

/**
   Define Sleep method.
*/
unsigned Sleep(unsigned milliseconds) 
{
#ifdef _POSIX_C_SOURCE
  if(_POSIX_C_SOURCE >= 199309L) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds * 1000) % 1000000;
    nanosleep(&ts, NULL);
  }
  else
  {
    usleep(milliseconds * 1000);
  }
#endif // _POSIX_C_SOURCE >= 199309L
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS
