/*
  New method allocator.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __NEW_ALLOCATOR_H
#define __NEW_ALLOCATOR_H

#include <CommProto/tools/allocator/allocator.h>
#include <new>
#include <memory>

namespace comnet {
namespace tools {
namespace allocator {

/**
   Standard NewAllocator is an object oriented way of allocating objects and data types. It is
   however, not very stable when it comes to handling the standard library pointers, so use this 
   allocator for convenient methods.
*/
template<typename _Ty>
class COMM_EXPORT NewAllocator : public Allocator<_Ty> {
  typedef _Ty& reference;
  typedef _Ty&& move_type;
  typedef const _Ty& const_reference;
  typedef const _Ty* const_pointer;
  typedef _Ty* pointer;
public:
  NewAllocator() { }

  ~NewAllocator() { }

  /**
     Allocate space.
   */
  pointer Allocate(uint32_t sizeN) {
    pointer p = (_Ty*)(::operator new(sizeN + sizeof(_Ty)));
    return p;
  }
  
  /**
     Deallocate space that was provided.
   */
  void Deallocate(pointer p) {
    if (p != nullptr) {
      ::operator delete((void*)p);
      p = nullptr;
    }
  }

  /**
     Destroy the object or data type that was inside the pointer.
   */
  void Destruct(pointer p) {
    p->~_Ty();
  }

  /**
     Construct the value in specified pointer.
   */
  void Construct(pointer p, const_reference value) {
    // TODO(Garcia): Tries to go NULL at first!! ??
    new((void *)p) _Ty();
    *p = std::move((reference)value);
  }

  /**
     Get the pointer from reference.
   */
  pointer Address(reference ref) {
    return &ref;
  }

  /**
     Get the constant pointer from constant reference.
   */
  const_pointer Address(const_reference ref) const {
    return &ref;
  }
private:
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace

#endif // __NEW_ALLOCATOR_H
