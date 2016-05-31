#include <architecture/os/os_mutex.h>

#if COM_TARGET_OS == COM_OS_WINDOWS
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
