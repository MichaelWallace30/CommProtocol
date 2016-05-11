#ifndef TEMP_H
#define TEMP_H

#include <Windows.h>
typedef HANDLE thread_t;

extern "C"
{
	/** Global C function to create thread*/
	void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg);	
	/** GLobal C function which returns thread id */
	thread_t thread_get_self_id();
}

#endif