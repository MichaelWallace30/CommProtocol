
#include "temp.h"

void thread_create(thread_t* thread, void *(*start_routine) (void *), void* arg)
{
	*thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
}

thread_t thread_get_self_id()
{
	return GetCurrentThread();
}