#ifndef __LINUX_CONFIG_H_
#define __LINUX_CONFIG_H_

#include <CommProtocol/architecture/os/arch.h>

#if (COM_TARGET_OS == COM_LINUX_OS)

#include <pthread.h>
/** Define Linux typedef of threading data type*/
typedef pthread_t thread_t;
#include <assert.h>

#if (COM_DISABLE_ASSERT > 0)
 #define COM_ASSERT(cond)
#else
 #define COM_ASSERT(cond)   assert(cond)
#endif

#define COM_UNUSED_PARAM(unusedparam)  (void)unusedparam

#ifndef NULL
 #ifdef __cplusplus
  #define NULL 0
 #else
  #define NULL ((void*)0)
 #endif
#endif

#endif // COM_TARGET_OS == COM_LINUX_OS

#endif
