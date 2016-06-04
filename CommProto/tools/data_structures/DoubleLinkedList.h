/*
  Double Linked List data structure.

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
#ifndef __DOUBLE_LINKED_QUEUE_H
#define __DOUBLE_LINKED_QUEUE_H

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
#include <CommProto/tools/StandardComparator.h>

#include <CommProto/architecture/macros.h>

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
_COMNET_PUBLIC_API_
template<typename _Ty,
	 class    _Compare = StandardComparator<_Ty> >
class DoubleLinkedList : public Interface::List<_Ty> {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef const _Ty const_data_type;
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

  void insert(const_reference value) {
    DNode* newNode = allocate_pointer(DNode);
    nullify_pointer(newNode->next);
    nullify_pointer(newNode->previous);
    newNode->data = value;

    if (this->size <= 0) {
      root = tail = newNode;
      newNode->index = this->size;
    } else {
      
    }

    this->size++;
  }

private:
  /**
     Node used as data containers within this data structure.
   */
  struct DNode {
    DNode* next;
    DNode* previous;
    
    _Ty data;
    uint32_t index;
  };

  DNode* root;
  DNode* tail;
};
} // DataStructures namespace 
} // Tools namespace
} // Comnet namespace
#endif // __DOUBLE_LINKED_QUEUE_H
