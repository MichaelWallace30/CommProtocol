// Thread handling header.
#ifndef __THREADING_H

#include <architecture/os/arch.h>

#if COM_TARGET_OS == COM_WINDOWS_OS
 #include <Windows.h>
 typedef HANDLE thread_t;
#else
 #include <pthread.h>
 #include <unistd.h>
 typedef pthread_t thread_t;
#endif // COM_TARGET_OS == COM_WINDOWS_OS

#endif // __THREADING_H
