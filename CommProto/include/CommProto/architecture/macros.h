/* 
  Macro configuration.

  Copyright (C) 2016  Michael Wallace, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MACROS_H
#define __MACROS_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

// TODO(Garcia) : Will need to move these in another file soon.
#define BIT16_VALUE 65536
#define BIT32_VALUE 4294967296

// A loose string copy as an alternative to 
// the C library strcpy.
// NOTE: You can only copy into dynamically allocated (malloc, or new)  strings (char*). 
_COMNET_PUBLIC_API_
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
_COMNET_PUBLIC_API_
#define str_length(str, length) {       \
          length = 0;                   \
	  while (str[length]) { \
	    length++;                   \
	  }         \
	}	


// Delete a list, consisting of the node data type, 
// and the variable "size" keeps track of the list.
// Node have to be linked in some way however.
_COMNET_PUBLIC_API_
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
_COMNET_PUBLIC_API_
#define nullify_pointer(pointer) { \
          pointer = NULL;          \
        }



// Simple, Standard data allocator.
_COMNET_PUBLIC_API_
#define allocate_pointer(data_type) new data_type()

// Simple allocation of a new pointer.
_COMNET_PUBLIC_API_
#define allocate_new_pointer(pointer, data_type) pointer = allocate_pointer(data_type)

// Simple, Standard free pointer.
_COMNET_PUBLIC_API_
#define free_pointer(pointer) {  \
          if (pointer != NULL) { \
	    delete pointer;      \
	    pointer = NULL;      \
	  }                      \
        }

_COMNET_PUBLIC_API_
// Check if number
#define is_number(string, result) { \
          result = true; \
          for (uint32_t i = 0; i < string.length(); ++i) { \
            if (!isdigit(string[i])) { \
              result = false; \
              break; \
            } \
          } \
        }

_COMNET_PUBLIC_API_
// Nullify the indices in the table.
#define nullifyAttributesInTable(theTable, size) { \
          for (uint32_t i = 0; i < size; ++i) { \
	    theTable[i] = NULL; \
	  } \
        }

#define cleanupListPointerAttributes(listPointer) { \
          for (uint32_t i = 0; i < listPointer->GetSize(); ++i) { \
            delete listPointer->At(i); \
          } \
        }
#endif // __MACROS_H
