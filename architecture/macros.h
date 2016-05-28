/*
  This file contains commonly used macros for this library.
*/
#ifndef __MACROS_H
#define __MACROS_H

#include <architecture/os/include-defines.h>

// A loose string copy as an alternative to 
// the C library strcpy.
#define loose_strcopy(dest, src, size) \
	{                              \
	  char* temp1 = (char*)dest;   \
	  int32_t i = 0;               \
	  while (i != size)            \
	  {                            \
	    temp1[i] = src[i++];       \
	  }                            \
	  dest = (const char*)temp1;   \
	}


#endif // __MACROS_H
