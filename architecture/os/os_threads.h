
#ifndef OSTHREADS_H
#define OSTHREADS_H

#include <architecture/os/include_defines.h>
#include <architecture/os/thread-config.h>

extern "C"
{

//posix \ window threading
  /* Aside from the function pointer taking void* as argument and returning void*, nothing special here 
*/
void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg);

thread_t thread_get_self_id();

#if COM_TARGET_OS != COM_OS_WINDOWS
unsigned Sleep(unsigned seconds);
#endif // COM_TARGET_OS != COM_OS_WINDOWS


}

#endif//OSTHREAD_H
