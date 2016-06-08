#ifndef __MAC_CONFIG_H_
#define __MAC_CONFIG_H_

#include <CommProto/architecture/os/arch.h>

#if (COM_TARGET_OS == COM_OS_APPLE)

#if (COM_DISABLE_ASSERT > 0)
 #define COM_ASSERT(cond)
#else
 #define COM_ASSERT(cond)        assert(cond)
#endif

#define COM_UNUSEDPARAM(unusedparam)  (void)unusedparam

/* Define NULL value */
#ifndef NULL
 #ifdef __cplusplus
  #define NULL 0
 #else
  #define NULL ((void*)0)
 #endif
#endif

#endif

#endif // __MAC_CONFIG_H_
