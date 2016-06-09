/*
  Stack Interface for CommProtocol.

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
#ifndef __INTERFACE_STACK_H
#define __INTERFACE_STACK_H

#include <CommProto/include/tools/data_structures/inteface/Generic.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

enum StackType {
  ARRAY_STACK,
  LIST_STACK,
};

/**
   Standard declaration for our stack data structures. 
 */
template<_Ty>
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
  DataStructureType getDataStructType() { return type; }

  /**
     Get the type of stack.
   */
  StackType getStackType() { return stackType; }

  /**
     Check if the stack is empty.
   */
  bool isEmpty() { return size == 0; }
  /**
     Get the current size of the stack.
   */
  int32_t getSize() { return size; }
  /**
     Get the top value, or the value that was last put inside the stack.
   */
  virtual const_reference getTopValue() = 0;

  /**
     Push a value into the stack.
   */
  virtual void push(const_reference value) = 0;
  /**
     Pop the last value out of the stack.
   */
  virtual void pop() = 0;
protected:
  /**
     The type of stack.
   */
  StackType stackType;
};

} // Interface namespace
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace
#endif // __INTERFACE_STACK_H
