#include <architecture/os/osThreads.h>
#include <architecture/os/arch.h>
//#include <architecture/os/thread-config.h>

#if COM_TARGET_OS == COM_OS_WINDOWS

void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) {
  *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
}

thread_t thread_get_self_id() {
  return GetCurrentThread();
}
#else

void thread_create(thread_t* thread, void* (*start_routine)(void*), void* arg) {
  pthread_create(thread, NULL, start_routine, arg);
}

thread_t thread_get_self_id() {
  return pthread_self();
}

unsigned Sleep(unsigned seconds) {
  return sleep((seconds / 1000));
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS
