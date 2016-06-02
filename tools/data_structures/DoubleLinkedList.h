#ifndef __DOUBLE_LINKED_QUEUE_H
#define __DOUBLE_LINKED_QUEUE_H

#include <tools/data_structures/interface/InterfaceList.h>
#include <architecture/macros.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   The double linked list is a special type of list, which will allow storage in
   O(1) time, while also allowing for a O(n) traverse, same as the SingleLinkedList.
   The DoubleLinkedList performs much better than the SingleLinkedList, however, becuase 
   of it's complex nature, which involves allowing the nodes to be traversed forwards,
   as well as backtrack to find the specified node.
 */
template<typename _Ty,
	 class    _Compare>
class DoubleLinkedList : public Interface::List<_Ty> {
public:
  
  DoubleLinkedList()
  : root(NULL)
  , tail(NULL) {
    listType = Interface::DOUBLE_LINKED_LIST;
    this->size = 0;
  }

  ~DoubleLinkedList() {
    _delete_list(DNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
  }

private:
  
  struct DNode {
    DNode* next;
    DNode* previous;
    
    _Ty data;
    uint32_t index;
  };
};
} // DataStructures namespace 
} // Tools namespace
} // Comnet namespace
#endif // __DOUBLE_LINKED_QUEUE_H
