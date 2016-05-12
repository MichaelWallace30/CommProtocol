#include <osThreads.h>

//really defineed in \architecture\os\include_defines.h and typedefs for thread_t are in specific OS -config.h files

/* Aside from the function pointer taking void* as argument and returning void*, nothing special here */
#if COM_TARGET_OS == COM_OS_WINDOWS //windows thread
void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg)
{
	*thread= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) start_routine, arg, 0, NULL);
}

thread_t thread_get_self_id()
{
    return GetCurrentThread();
}

#else //posix threads
void thread_create(thread_t* thread, void* (*start_routine) (void *), void* arg)
{
	pthread_create(thread, NULL,start_routine, arg);
}

thread_t thread_get_self_id()
{
    return pthread_self();
}

#endif
