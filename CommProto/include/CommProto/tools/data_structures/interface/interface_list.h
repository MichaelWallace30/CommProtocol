/*
  List Interface for CommProtocol.

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
#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

#include <CommProto/tools/data_structures/interface/generic.h>

//using namespace Comnet::Tools::DataStructures::Interface;

namespace comnet {
namespace tools {
namespace datastructures {
namespace interface {

/** 
    ListType Contains value of the data structure being used. 
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
class COMM_EXPORT List : public Generic {
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
  DataStructureType GetDataStructType() { return type; }
  /** 
      Grab the type of list this data structure is. 
  */
  ListType GetListType() { return list_type; }
  /** 
      Check if this data structure is empty. 
      @returns True if empty, false otherwise.
  */
  virtual bool IsEmpty() { return (size == 0); }

  /**
      Get the number of values in the data structure.
  */
  virtual int32_t GetSize() { return size; }

  /**
     Insert a value into the list.
  */
  virtual void Insert(const _Ty& value) = 0;
  /** 
      Remove a value from the list.
  */
  virtual bool Remove(const _Ty& value) = 0;
  /**
     Remove a value from a specified index.
  */
  virtual bool RemoveAt(const int32_t index) = 0;
  /**
     Check the first index of this list.
  */
  virtual reference_type Front() = 0;
  /**
     Check the last index of this list.
  */
  virtual reference_type Back() = 0;
  /**
     Grab the value At the specified index.
  */
  virtual reference_type At(const int32_t index) = 0;
  /**
     Check if the list Contains this value.
  */
  virtual bool Contains(const _Ty& value) = 0;
protected:

  /** 
      The type of list this data structure is.
  */
  ListType list_type;
};
} // Interface
} // DataStructures
} // Tools
} // Comnet
#endif // __INTERFACE_LIST_H
