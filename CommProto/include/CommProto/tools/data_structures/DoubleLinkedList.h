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
#ifndef __DOUBLE_LINKED_LIST_H
#define __DOUBLE_LINKED_LIST_H

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
	 class    _Compare = Comparator<_Ty>,
	 class    _Alloc   = Tools::Allocator::Allocator<_Ty> >
class DoubleLinkedList : public Interface::List<_Ty> {
  /*
    TODO(Garcia): We will need to make use of the Allocator.
   */
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef const _Ty const_data_type;
  typedef _Ty& reference_type;
  /**
  Node used as data containers within this data structure.
  */
  struct DNode {
    DNode* next;
    DNode* previous;

    _Ty data;
    uint32_t index;
  };
public:
  /**
     Default Constructor for data structure. 
   */
  template<typename _Comparator = StandardComparator<_Ty>,
	   typename _Allocator  = Tools::Allocator::NewAllocator<_Ty> >
  DoubleLinkedList(_Comparator comparator = _Comparator(), _Allocator allocator = _Allocator() )
  : root(NULL)
  , tail(NULL)
  , cursor(NULL)
  , alloc(allocator) 
  , cmp(comparator)
  {
    this->listType = Interface::DOUBLE_LINKED_LIST;
    this->size = 0;
  }

  /**
     Default destructor for the data structure.
   */
  ~DoubleLinkedList() {
    _delete_list(DNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
    nullify_pointer(cursor);
  }
  /**
     Inserts a value into the data structure.
   */
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
  
  /**
     Handles the setup to remove the root value.
   */
  DNode* handleRootRemoval(DNode* remNode) {
    remNode = root;
    root = root->next;
    
    if (cursor == remNode) {
      cursor = root;
    }

    if (tail == remNode) {
      tail = tail->next;
    }
    
    if (root != NULL) {
      nullify_pointer(root->previous);
    }
    return remNode;
  }
  /**
     Handles the setup to remove the tail value.
   */
  DNode* handleTailRemoval(DNode* remNode) {
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
    return remNode;
  }

  /**
     Handles the setup to remove the cursor.
   */
  DNode* handleCursorRemoval(DNode* remNode) {
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
    
    return remNode;
  }

  /**
     Handles the setup to remove the selected node. Indices are reorganized so 
     as to keep the stability of the index for each remaining nodes in the data structure.
   */
  inline
  void handleRemNodeDelete(DNode* remNode) {
    DNode* traverse = remNode->next;
    for (int i = remNode->index; i < this->size - 1; ++i) {
      traverse->index = i;
      traverse = traverse->next;
    }
    
    free_pointer(remNode);
  }
  

public:
  /**
     Removes a node based on the specified value. This may be a greedy method, considering it will not
     remove all nodes similar to specified value.
   */
  comm_bool remove(const_reference value) {
    comm_bool success = comm_bool::_FALSE;
    if (this->isEmpty()) {
      return success;
    }

    DNode* remNode = NULL;
    
    if (cmp.equal(root->data, value)) {
      remNode = handleRootRemoval(remNode);
    } else if (cmp.equal(tail->data , value)) {
      remNode = handleTailRemoval(remNode);
    } else if (cmp.equal(cursor->data, value)) {
      remNode = handleCursorRemoval(remNode);
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
      handleRemNodeDelete(remNode);
      this->size--;
      success = comm_bool::_TRUE;
    }

    return success;
  }

  /**
     Remove a node from the list on the specified index.
   */
  comm_bool removeAt(const int32_t index) {
    comm_bool success = comm_bool::_FALSE;
    if (index >= this->size || index < 0) {
      return success;
    }

    DNode* remNode;
    nullify_pointer(remNode);
    
    if (index == root->index) {
      remNode = handleRootRemoval(remNode);
    } else if (index == tail->index) {
      remNode = handleTailRemoval(remNode);
    } else if (index == cursor->index) {
      remNode = handleCursorRemoval(remNode);
    } else {
      if (cursor->index > index) {
	cursor = cursor->previous;
	while (cursor != NULL) {
	  if (cursor->index == index) {
	    remNode = cursor;
	    cursor = cursor->next;
	    
	    cursor->previous = remNode->previous;
	    remNode->previous->next = cursor;
	    break;
	  }
	  cursor->previous;
	}
      } else {
	cursor = cursor->next;
	while (cursor != NULL) {
	  if (cursor->index == index) {
	    remNode = cursor;
	    cursor = cursor->next;
	    
	    cursor->previous = remNode->previous;
	    remNode->previous->next = cursor;
	    break;
	  }
	  cursor = cursor->next;
	}
      }
    }

    if (remNode != NULL) {
      handleRemNodeDelete(remNode);
      this->size--;
      success = comm_bool::_TRUE;
    }
    
    return success;
  }

  /**
     Get the front root value of this data structure.
   */
  reference_type front() {
    return root->data;
  }
  /**
     Get the back, tail, value of this data structure.
   */
  reference_type back() { 
    return tail->data;
  }

  reference_type at(const int32_t index) {
    if (root->index == index) {
      return root->data;
    } else if (tail->index == index) {
      return tail->data;
    } else if (cursor->index == index) {
      return cursor->data;
    } else {
      if (cursor->index > index) {
	cursor = cursor->previous;
	while (cursor != NULL) {
	  if (cursor->index == index) {
	    return cursor->data;
	  } else {
	    cursor = cursor->previous;
	  }
	}
      } else {
	cursor = cursor->next;
	while (cursor != NULL) {
	  if (cursor->index == index) {
	    return cursor->data;
	  } else {
	    cursor = cursor->next;
	  }
	}
      }
    }
  }

  /**
     Checks if the specified value is in this data structure.
   */
  comm_bool contains(const_reference value) {
    comm_bool success = comm_bool::_FALSE;
    
    if (cmp.equal(root->data, value) ||
	cmp.equal(tail->data, value) ||
	cmp.equal(cursor->data, value)) {
      success = comm_bool::_TRUE;
    } else {
      cursor = root->next;
      while (cursor != NULL) {
	if (cmp.equal(cursor->data, value)) {
	  success = comm_bool::_TRUE;
	  break;
	}
	cursor = cursor->next;
      }
    }

    return success;
  }
  /**
     Get the cursor value.
   */
  const _Ty& getCurrent() {
    return cursor->data;
  }
private:
  /**
     Root node, the front, head, of the data structure.
   */
  DNode* root;
  /**
     Tail node, the back, tail, of the data structure.
   */
  DNode* tail;
  /**
     Cursor node, the current node in the data structure.
   */
  DNode* cursor;
  /**
     Compare object, used to compare values in the data structure. This can be customized as to 
     make this structure more dynamic.
   */
  _Compare& cmp;
  /**
     Allocator object, used to allocate values in the data structure. This can be customized as to make this
     structure more dynamic.
   */
  _Alloc&  alloc;
};
} // DataStructures namespace 
} // Tools namespace
} // Comnet namespace
#endif // __DOUBLE_LINKED_LIST_H
