#include <CommProto/include/architecture/os/os_threads.h>


/*
  Define the target OS. Windows and Unix like systems use different APIs.
*/
#if COM_TARGET_OS == COM_OS_WINDOWS

/**
   Windows definition to create a thread.
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
#if _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds * 1000) % 1000000;
  nanosleep(&ts, NULL);
#else
  usleep(milliseconds * 1000);
#endif // _POSIX_C_SOURCE >= 199309L
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS
