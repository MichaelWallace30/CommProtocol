#include <architecture/os/osThreads.h>


/*
  Define the target OS. Windows and Unix like systems use different APIs.
*/
#if COM_TARGET_OS == COM_OS_WINDOWS

/**
   Windows definition to create a thread.
*/
void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) {
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
}

/**
   Get the id of the current thread.
*/
thread_t thread_get_self_id() {
  return GetCurrentThread();
}
#else

/**
   Create a pthread.
*/
void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) {
  pthread_create(thread, NULL, start_routine, arg);
}

/**
   Get the current thread id.
*/
thread_t thread_get_self_id() {
  return pthread_self();
}

/**
   Define Sleep method.
*/
unsigned Sleep(unsigned seconds) {
  return sleep((seconds / 1000));
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS
