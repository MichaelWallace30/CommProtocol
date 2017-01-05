/*
  Specialization of doubly link list that is thread safe.

  Copyright (C) 2016 Alex Craig.

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

#ifndef __THREAD_SAFE_LIST_H
#define __THREAD_SAFE_LIST_H

#include <CommProto/tools/data_structures/double_linked_list.h>
#include <CommProto/architecture/os/comm_mutex.h>

using namespace comnet::architecture::os;

namespace comnet {
namespace tools {
namespace datastructures {

/**
  A thread safe implementation of the doubly linked list.
*/
_COMNET_PUBLIC_API_
  template<typename _Ty,
  class    _Compare = StandardComparator<_Ty>,
  class    _Alloc = tools::allocator::NewAllocator<_Ty> >
class COMM_EXPORT ThreadSafeList : public DoubleLinkedList <_Ty> {
  typedef const _Ty& const_reference;
  typedef _Ty& reference_type;
public:
  /**
    Default Constructor for data structure.
  */
  ThreadSafeList(const _Compare& comparator = _Compare(), const _Alloc& allocator = _Alloc())
    : DoubleLinkedList<_Ty>(comparator, allocator)
  {
          
  }

  ThreadSafeList(const _Alloc& allocator)
    : DoubleLinkedList<_Ty>(allocator)
  {

  }

  /**
    Inserts a value into the data structure.  Thread safe.
  */
  void Insert(const_reference value) override {
    CommLock lock(listMutex);
    DoubleLinkedList<_Ty>::Insert(value);
  }

  /**
   Removes a node based on the specified value.  May be greedy because it will not remove all 
   nodes similar to the value.  Thread safe.
  */
  bool Remove(const_reference value) override {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::Remove(value);
  }

  /**
    Remove a node from the list on the specified index.  Thread safe.
  */
  bool RemoveAt(const int32_t index) override {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::RemoveAt(index);
  }

  /**
    Get the root value of the list.  Thread safe.
  */
  reference_type Front() override {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::Front();
  }

  /**
    Get the last/tail value of the list.  Thread safe.
  */
  reference_type Back() override {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::Back();
  }

  /**
    Iterates to the index specified and returns it's value.  Thread safe.
  */
  reference_type At(const int32_t index) override {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::At(index);
  }

  /**
    Checks if the specified value is in this data structure.  Thread safe.
  */
  bool Contains(const_reference value) {
    CommLock lock(listMutex);
    return DoubleLinkedList<_Ty>::Contains(value);
  }

  /**
    Get the cursor value. Thread safe.
  */
  const _Ty& GetCurrent() {
    CommLock lock(listMutex);
    return *this->cursor->data;
  }

  /**
    Default destructor for the data structure.
  */
  ~ThreadSafeList()
  {

  }
        
private:
  /**
    Prevents more than one thread from reading and modifyig the list.
  */
  CommMutex listMutex;
};
} //datastructures namespace
} //tools namespace
} //comnet namespace
#endif  // __THREAD_SAFE_LIST_H
