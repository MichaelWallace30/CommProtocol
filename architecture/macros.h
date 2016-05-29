/*
  This file contains commonly used macros for this library.
*/
#ifndef __MACROS_H
#define __MACROS_H

#include <architecture/os/include-defines.h>

// A loose string copy as an alternative to 
// the C library strcpy.
// NOTE: You can only copy into dynamically allocated (malloc, or new)  strings (char*). 
#define loose_strcopy(dest, src, size) \
	{                              \
	  int32_t i = 0;               \
	  while (i != size)            \
	  {                            \
	    dest[i] = src[i++];        \
	  }                            \
	  dest[0] = '\0';              \
	}


#endif // __MACROS_H
