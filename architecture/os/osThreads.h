
#ifndef OSTHREADS_H
#define OSTHREADS_H

#include <architecture/os/include_defines.h>
#include <architecture/os/thread-config.h>

//posix \ window threading

extern "C" {		
/* Aside from the function pointer taking void* as argument and returning void*, nothing special here */
#if COM_TARGET_OS == COM_OS_WINDOWS //windows thread

static void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg)
{
	*thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
}

static thread_t thread_get_self_id()
{
	return GetCurrentThread();
}

#else  //posix threads

static void thread_create(thread_t* thread, void* (*start_routine) (void *), void* arg)
{
	pthread_create(thread, NULL, start_routine, arg);
}

static thread_t thread_get_self_id()
{
	return pthread_self();
}

unsigned Sleep(unsigned seconds) 
{
	return sleep(seconds);
}

#endif
}
#endif//OSTHREAD_H
