/*
  Stack Interface for CommProtocol.

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
#ifndef __INTERFACE_STACK_H
#define __INTERFACE_STACK_H

#include <CommProto/tools/data_structures/interface/generic.h>

namespace comnet {
namespace tools {
namespace datastructures {
namespace interface {

enum StackType {
  ARRAY_STACK,
  LIST_STACK,
};

/**
   Standard declaration for our stack data structures. 
 */
template<typename _Ty>
class Stack : public Generic {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty& reference_type;
public:
  /**
     Constructor for the stack.
   */
  Stack() { type = STACK; }
  /**
     This class is polymorphic, so we can inherit from it.
   */
  virtual ~Stack() { }
  /**
     Get the data structure type.
   */
  DataStructureType GetDataStructType() { return type; }

  /**
     Get the type of stack.
   */
  StackType GetStackType() { return stack_type; }

  /**
     Check if the stack is empty.
   */
  bool IsEmpty() { return (size == 0); }
  /**
     Get the current size of the stack.
   */
  int32_t GetSize() { return size; }
  /**
     Get the top value, or the value that was last put inside the stack.
   */
  virtual const_reference GetTopValue() = 0;

  /**
     Push a value into the stack.
   */
  virtual void Push(const_reference value) = 0;
  /**
     Pop the last value out of the stack.
   */
  virtual void Pop() = 0;
protected:
  /**
     The type of stack.
   */
  StackType stack_type;
};

} // Interface namespace
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace
#endif // __INTERFACE_STACK_H
