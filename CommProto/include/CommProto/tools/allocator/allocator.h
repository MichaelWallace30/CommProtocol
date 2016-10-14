/*
  Allocator interface.

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
#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/api.h>

namespace comnet {
namespace tools {
namespace allocator {

/**
   Base class Allocator, which allows users to inherit from, creating their own allocators
   and cleaning them up as they wish. Allocator is inherited by all allocator types, in which
   will be used for the purpose of this library. Simply extend this interface to your allocator and 
   build your allocator based on the needs for your application, or for this library.
 */
template<typename _Ty>
class COMM_EXPORT Allocator {
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty* pointer;
public:
  /**
     Base Constructor for this allocator.
   */
  Allocator() { }
  /**
     Polymorphic destructor for this allocator base class.
   */
  virtual ~Allocator() { }
  /**
     Allocate memory for your object, or data.
   */
  virtual pointer Allocate(uint32_t size_n) = 0;

  /**
     Deallocate memory, which will return that memory Back to the operating system.
   */
  virtual void Deallocate(pointer p) = 0;
  /**
     Destroy the object, this will not Deallocate your memory however. IF memory was allocated on the heap,
     be sure to destroy it first before deallocating.
   */
  virtual void Destruct(pointer p) = 0;
  /**
     Construct value by move constructor into your allocated pointer. 
     Be sure to Allocate first before constructing.
   */
  virtual void Construct(pointer p, const_reference value) = 0;

  /**
     Grab the pointer to a value.
   */
  virtual pointer Address(reference ref) = 0;

  /**
     Get the pointer to a value.
   */
  virtual const_pointer Address(const_reference ref) const = 0;
private:
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace

#endif // __ALLOCATOR_H
