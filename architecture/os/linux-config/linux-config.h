#ifndef __LINUX_CONFIG_H_
#define __LINUX_CONFIG_H_

#include <architecture/os/arch.h>

#if (COM_TARGET_OS == COM_OS_LINUX)

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

#endif // COM_TARGET_OS == COM_OS_LINUX

#endif
