#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

#include <tools/data_structures/interface/Generic.h>

namespace Comnet::DataStructure::Package;

namespace Comnet {
namespace DataStructure {

enum ListType {
  SINGLE_LINKED_LIST,
  DOUBLE_LINKED_LIST,
  CIRCULAR_LINKED_LIST,
  VECTOR_LIST,
  ARRAY_LIST,
};

template<typename _Ty>
class List : public Interface {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty& reference_type;
public:
  List() { }
  virtual ~List() { }
  
  DataStructureType getDataStructType() { return type; }
  ListType getListType() { return listType; }
  bool isEmpty() { return size; }

  virtual void insert(_Ty value) = 0;
  virtual reference_type remove(_Ty& value) = 0;
  virtual reference_type removeAt(const int32_t index) = 0;
  virtual reference_type front() = 0;
  virtual reference_type at(const int32_t index) = 0;
  virtual reference_type operator[](const int32_t index) = 0;
private:

  ListType listType;
};
} // DataStructure
} // Comnet
#endif // __INTERFACE_LIST_H
