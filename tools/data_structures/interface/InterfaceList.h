#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

#include <tools/data_structures/interface/Generic.h>

using namespace Comnet::DataStructure::Package;

namespace Comnet {
namespace DataStructure {

/** ListType contains value of the data structure being used. */
enum ListType {
  SINGLE_LINKED_LIST,
  DOUBLE_LINKED_LIST,
  CIRCULAR_LINKED_LIST,
  VECTOR_LIST,
  ARRAY_LIST,
};

/** ListInterface provides an interface that is common with all
    list data structures. It helps if we may need to store data
    structures together for certain functionality. */
template<typename _Ty>
class List : public Interface {
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
  bool isEmpty() { return size == 0; }

  /**
      Get the number of values in the data structure.
  */
  int32_t getSize() { return size; }

  /**
     Insert a value into the list.
  */
  virtual void insert(_Ty value) = 0;
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
} // DataStructure
} // Comnet
#endif // __INTERFACE_LIST_H
