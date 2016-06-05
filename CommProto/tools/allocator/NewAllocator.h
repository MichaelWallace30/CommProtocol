/*
  New method allocator.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __NEW_ALLOCATOR_H
#define __NEW_ALLOCATOR_H

#include <stdlib.h>

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

namespace Comnet {
namespace Tools {
namespace Allocator {

/**
   Standard NewAllocator is an object oriented way of allocating objects and data types. It is
   however, not very stable when it comes to handling the standard library pointers, so use this 
   allocator for convenient methods.
*/
template<typename _Ty>
class NewAllocator {
  typedef _Ty& reference;
  typedef const _Ty& const_reference;
  typedef const _Ty* const_pointer;
  typedef _Ty* pointer;
public:
  NewAllocator() { }

  ~NewAllocator() { }

  /**
     Allocate space.
   */
  pointer allocate(uint32_t sizeN) {
    pointer p = (_Ty*)(::operator new(sizeN * sizeof(_Ty)));
    return p;
  }
  
  /**
     Deallocate space that was provided.
   */
  void deallocate(pointer p) {
    if (p != NULL) {
      ::operator delete((void*)p);
      p = NULL;
    }
  }

  /**
     Destroy the object or data type that was inside the pointer.
   */
  void destruct(pointer p) {
    p->~_Ty();
  }

  /**
     Construct the value in specified pointer.
   */
  void construct(pointer p, const_reference value) {
    new((void*)p)_Ty(value);
  }

  /**
     Get the pointer from reference.
   */
  pointer address(reference ref) {
    return &ref;
  }

  /**
     Get the constant pointer from constant reference.
   */
  const_pointer address(const_reference ref) const {
    return &ref;
  }
private:
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace

#endif // __NEW_ALLOCATOR_H
