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
#include <CommProto/tools/allocator/NewAllocator.h>

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
	 class    _Compare = StandardComparator<_Ty>,
	 class    _Alloc = Tools::Allocator::NewAllocator<_Ty> >
class DoubleLinkedList : public Interface::List<_Ty> {
  /*
    TODO(Garcia): We will need to make use of the Allocator.
   */
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef const _Ty const_data_type;
public:
  
  DoubleLinkedList()
  : root(NULL)
  , tail(NULL)
  , cursor(NULL) {
    listType = Interface::DOUBLE_LINKED_LIST;
    this->size = 0;
  }

  ~DoubleLinkedList() {
    _delete_list(DNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
    nullify_pointer(cursor);
  }

  void insert(const_reference value) {
    DNode* newNode = allocate_pointer(DNode);
    nullify_pointer(newNode->next);
    nullify_pointer(newNode->previous);
    newNode->data = value;

    if (this->size <= 0) {
      root = tail = cursor = newNode;
    } else {
      tail->next = newNode;
      newNode->previous = tail;
      tail = newNode;
    }

    newNode->index = this->size++;
  }

private:
  
  void handleRootRemoval(DNode* remNode) {
    remNode = root;
    root = root->next;
    
    if (cursor == remNode) {
      cursor = root;
      nullify_pointer(cursor->previous);
    }

    if (tail == remNode) {
      tail = tail->next;
      nullify_pointer(tail->previous);
    }

    nullify_pointer(root->previous);
  }
  
  void handleTailRemoval(DNode* remNode) {
    remNode = tail;
    tail = tail->previous;

    if (cursor == remNode) {
      cursor = cursor->previous;
      nullify_pointer(cursor->next);
    }

    if (root == remNode) {
      root = root->previous;
      nullify_pointer(root->next);
    }

    nullify_pointer(tail->next);
  }

  void handleCursorRemoval(DNode* remNode) {
    remNode = cursor;
    
    if (cursor == tail) {
      cursor = cursor->previous;
  
      nullify_pointer(cursor->next);
    } else if (cursor == root) {
      cursor = cursor->next;
      root = root->next;

      nullify_pointer(cursor->previous);
      nullify_pointer(root->previous);
    } else {
      cursor = cursor->next;
      cursor->previous = remNode->previous;
      remNode->previous->next = cursor;
    }
  }
  

public:
  bool remove(cconst_reference value) {
    bool success = false;
    if (this->isEmpty()) {
      return success;
    }

    DNode* remNode = NULL;
    
    if (cmp.equal(root->data, value)) {
      handleRootRemoval(remNode);
    } else if (cmp.equal(tail->data , value)) {
      handleTailRemoval(remNode);
    } else if (cmp.equal(cursor->data, value)) {
      handleCursorRemoval(remNode);
    } else {
      cursor = root->next;
      
      while (cursor != tail) {
	if (cmp.equal(cursor->data , value)) {
	  remNode = cursor;
	  cursor = cursor->next;
	  
	  cursor->previous = remNode->previous;
	  remNode->previous->next = cursor;
	  break;
	}
	
	cursor = cursor->next;
      }
    }

    if (remNode != NULL) {
      DNode* traverse = remNode->next;
      for (int i = remNode->index; i < this->size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }

      free_pointer(remNode);
      this->size--;
      success = true;
    }

    return success;
  }

  bool removeAt(const int32_t index) {
    bool success = false;
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
  DNode* cursor;

  _Compare cmp;
  _Alloc  alloc;
};
} // DataStructures namespace 
} // Tools namespace
} // Comnet namespace
#endif // __DOUBLE_LINKED_QUEUE_H
