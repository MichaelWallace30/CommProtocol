#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

#include <CommProtocol/tools/data_structures/interface/Generic.hpp>

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

class List : public Interface { 
  
};
} // DataStructure
} // Comnet
#endif // __INTERFACE_LIST_H
