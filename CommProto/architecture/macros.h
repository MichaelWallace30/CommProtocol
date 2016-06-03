/*
  This file contains commonly used macros for this library.
*/
#ifndef __MACROS_H
#define __MACROS_H

#include <CommProto/architecture/os/include_defines.h>

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
	  dest[i] = '\0';              \
	}

// Count the number of characters in the string.
#define str_length(str, length) {       \
          length = 0;                   \
	  while (str[length] != NULL) { \
	    length++;                   \
	  }                             \
        }


// Delete a list, consisting of the node data type, 
// and the variable "size" keeps track of the list.
// Node have to be linked in some way however.
#define _delete_list(nodeType, currentSize) { \
          if (currentSize > 0) {	      \
	    nodeType* previous;		      \
	    nodeType* traverse = root;	      \
	    while (traverse != NULL) {	      \
	      previous = traverse;	      \
	      traverse = traverse->next;      \
	      delete previous;                \
	      previous = NULL;                \
	    }				      \
          }                                   \
        }

// Nullify the pointer.
#define nullify_pointer(pointer) { \
          pointer = NULL;          \
        }
#endif // __MACROS_H
