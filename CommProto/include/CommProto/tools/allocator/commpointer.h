/*
  Standard library smart pointer.

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
#ifndef __COMM_POINTER_H
#define __COMM_POINTER_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/allocator/newallocator.h>
#include <CommProto/tools/data_structures/double_linked_list.h>

namespace comnet {
namespace tools {
namespace allocator {

/**
   Pointer handler to take care of possible memory leaks and ensures that the last pointer referencing
   the object will be deleted. Keep in mind, however, if two pointers reference the same object, if one
   of the pointers are deleted, the other pointer will be nullified. Be sure to reference the pointer, that 
   first referenced the object, if you wish to add a reference to the same object.
 */
template<typename _Ty,
	 class    _Alloc = NewAllocator<_Ty> >
class CommPointer {
  typedef _Ty* pointer_t;
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
public:
  CommPointer() 
    : pointer(NULL)
    , is_unique(true) { }
  
  CommPointer(const_reference ref)
    : is_unique(true) 
  {
    pointer = allocator.Allocate(1);
    allocator.Construct(pointer, ref);
  }

  CommPointer(const_pointer ptr)
    : is_unique(true) 
  {
    pointer = allocator.Allocate(1);
    allocator.Construct(pointer, *ptr);
  }
  
  CommPointer(CommPointer<_Ty>& ptr) 
  {
    ptr.InsertPointerReference(this);
    InsertPointerReference(&ptr);
    pointer = &((reference)ptr.Get());    
  }

  ~CommPointer() {
    if (is_unique) {
      allocator.Destruct(pointer);
      allocator.Deallocate(pointer);
    } else {
      pointer = NULL;
      // polynomial time of O(n), this may be an issue.
      for (int32_t i = 0; i < pointers.GetSize(); ++i) {
	pointers.At(i)->RemovePointerReference(this);
      }
    }
  }

  reference Get() {
    return *pointer;
  }

  int32_t GetNumberOfPointers() {
    return pointers.GetSize();
  }
  
protected:
  void InsertPointerReference(CommPointer<_Ty>* ref) {
    pointers.Insert(ref);
    if (!pointers.IsEmpty()) {
      is_unique = false;
    }
  }

  void RemovePointerReference(CommPointer<_Ty>* ref) {
    pointers.Remove(ref);
    if (pointers.IsEmpty()) {
      is_unique = true;
    }
  }

  bool is_unique;
private:
  
  pointer_t pointer;
  _Alloc allocator;

  tools::datastructures::DoubleLinkedList<CommPointer<_Ty>* > pointers;
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace

#endif // __COMM_POINTER_H
