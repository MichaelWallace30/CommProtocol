#ifndef __MUTEX_CONFIG_H
#define __MUTEX_CONFIG_H

#include <CommProto/include/architecture/os/arch.h>

#if COM_TARGET_OS == COM_OS_WINDOWS
 #include <Windows.h>
 typedef HANDLE mutex_t;
#else
 #include <pthread.h>
 #include <unistd.h>
 typedef pthread_mutex_t mutex_t;
#endif // COM_TARGET_OS == COM_OS_WINDOWS
#endif // __MUTEX_CONFIG_H
