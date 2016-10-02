/*
  Stack method  allocator.

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
#ifndef __STACK_ALLOCATOR_H
#define __STACK_ALLOCATOR_H

#include <CommProto/tools/allocator/allocator.h>

namespace comnet {
namespace tools {
namespace allocator {
/**
   StackAllocator works like a stack, push in some memory, and then pop out some of it.
   It works by allocating given space, from the heap, for the user to use. In order to successfully
   achieve fast performance, without doing anything dangerous such as allocating on the stack without
   proper memory management, be sure to malloc a certain amount of space for use.
   
   malloc *
   ------------------------------------------------------------------------------------------
   |    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
   ------------------------------------------------------------------------------------------
   ^ 
   pointer

   StackAllocator
   ------------------------------------------------------------------------------------------
   |    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
   ------------------------------------------------------------------------------------------
   ^      ^
   |	  pointer pushed
   memory returned
*/
template<typename _Ty>
class StackAllocator : public Allocator<_Ty>  {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty& reference;
  typedef _Ty* pointer;
public:
  StackAllocator(void* start)
    : __current_pos(start)
    , __start_pos(start)
    , __size(0)
    , __used_memory(0)
    , __num_allocations(0)
  {
  }

  pointer Allocate(uint32_t size_n) {
    __size += size_n;
    _Ty* _pointer = (_Ty*)__current_pos;
    __current_pos = (void*)((int32_t)__current_pos + size_n);
    return _pointer;
  }

  void Deallocate(pointer p) {
    
  }

  void Destruct(pointer p) {
  }
  
  void Construct(pointer p, const_reference value) {
  }
  
  pointer Address(reference ref) {
  }

  const_pointer address(const_reference ref) {
  }
private:
  void*   __start_pos;
  void*   __current_pos;
  int32_t __size;
  int32_t __used_memory;
  int32_t __num_allocations;
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace
#endif // __STACK_ALLOCATOR_H
