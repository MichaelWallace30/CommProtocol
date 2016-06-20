/*
  List Interface for CommProtocol.

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
#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

#include <CommProto/tools/data_structures/interface/Generic.h>

//using namespace Comnet::Tools::DataStructures::Interface;

namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

/** 
    ListType contains value of the data structure being used. 
*/
_COMNET_PRIVATE_API_
enum ListType {
  SINGLE_LINKED_LIST,
  DOUBLE_LINKED_LIST,
  CIRCULAR_LINKED_LIST,
  VECTOR_LIST,
  ARRAY_LIST,
  AUTO_VECTOR,
};

/** 
    ListInterface provides an interface that is common with all
    list data structures. It helps if we may need to store data
    structures together for certain functionality. 
*/
_COMNET_PRIVATE_API_
template<typename _Ty>
class List : public Generic {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty& reference_type;
public:
  /** 
      Constructor for the interface. 
  */
  List() { type = LIST; }
  /** 
      This class is polymorphic, so we can inherit from it. 
  */
  virtual ~List() { }
  
  /** 
      Grab the data structure type to check what it is. 
  */
  DataStructureType getDataStructType() { return type; }
  /** 
      Grab the type of list this data structure is. 
  */
  ListType getListType() { return listType; }
  /** 
      Check if this data structure is empty. 
      @returns True if empty, false otherwise.
  */
  virtual bool isEmpty() { return (size == 0); }

  /**
      Get the number of values in the data structure.
  */
  virtual int32_t getSize() { return size; }

  /**
     Insert a value into the list.
  */
  virtual void insert(const _Ty& value) = 0;
  /** 
      Remove a value from the list.
  */
  virtual bool remove(const _Ty& value) = 0;
  /**
     Remove a value from a specified index.
  */
  virtual bool removeAt(const int32_t index) = 0;
  /**
     Check the first index of this list.
  */
  virtual reference_type front() = 0;
  /**
     Check the last index of this list.
  */
  virtual reference_type back() = 0;
  /**
     Grab the value at the specified index.
  */
  virtual reference_type at(const int32_t index) = 0;
  /**
     Check if the list contains this value.
  */
  virtual bool contains(const _Ty& value) = 0;
protected:

  /** 
      The type of list this data structure is.
  */
  ListType listType;
};
} // Interface
} // DataStructures
} // Tools
} // Comnet
#endif // __INTERFACE_LIST_H
