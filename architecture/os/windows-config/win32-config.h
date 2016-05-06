#ifndef __WIN32_CONFIG_H
#define __WIN32_CONFIG_H

#include <CommProtocol/architecture/os/arch.h>

#if (COM_TARGET_OS == COM_OS_WINDOWS)


#include <assert.h>

#if (COM_DISABLE_ASSERT > 0)
 #define COM_ASSERT(cond)
#else
 #define COM_ASSERT(cond)             assert(cond)
#endif
#define COM_UNUSED_PARAM(unusedparam) (void)unusedparam


/* Define NULL pointer value */
#ifndef NULL
 #ifdef __cplusplus
  #define NULL 0
 #else
  #define NULL ((void*)0)
 #endif
#endif

#endif // COM_TARGET_OS == COM_OS_WINDOWS 

#endif // __WIN32_CONFIG_H_
