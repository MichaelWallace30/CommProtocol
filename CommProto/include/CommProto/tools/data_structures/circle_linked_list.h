/*
  Circle linked list.

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
#ifndef __CIRCLE_LINKED_LIST_H
#define __CIRCLE_LINKED_LIST_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/data_structures/interface/interface_list.h>
#include <CommProto/tools/standard_comparator.h>
#include <CommProto/tools/allocator/newallocator.h>

namespace comnet {
namespace tools {
namespace datastructures {

/**
   Circle linked list acts like a double linked list, except the fact that the tail
   references Back to the head of the list. This is a good data structure to use for 
   holding information in which either need to be constantly looked over, allowing the 
   program to switch between processes or threads, allowing the user to move between different
   programs and much more.
 */
template<typename _Ty,
	 typename _Compare = StandardComparator<_Ty>,
	 typename _Alloc   = allocator::NewAllocator<_Ty> >
class COMM_EXPORT CircleLinkedList : public interface::List<_Ty> {
  typedef _Ty* pointer;
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  /**
     CNode struct, used as containers for the list.
   */
  class CNode {
  public:
    CircleLinkedList<_Ty, _Compare, _Alloc>& ref;
    CNode* next;
    CNode* previous;
    
    _Ty* data;
    int32_t index;
    
    CNode(CircleLinkedList<_Ty, _Compare, _Alloc>& r)
      : next(NULL)
      , previous(NULL)
      , data(NULL)
      , index(0)
      , ref(r)
    { }
    
    void AllocateValue(const _Ty& value) {
      data = ref.alloc.Allocate(0);
      ref.alloc.Construct(data, value);
    }

    void DeallocateValue() {
      ref.alloc.Destruct(data);
      ref.alloc.Deallocate(data);
    }

    ~CNode() {
      DeallocateValue();
    }
  };

  /**
     Handle the removal of the root. This will effectively return the 
     node that is going to be removed, while also leaving the list safe.
   */
  CNode* HandleRootRemoval(CNode* remNode) {
    CNode* next = root->next;
    CNode* previous = root->previous;
    remNode = root;

    if ((root->next == root) &&
	(root->previous == root)) {
      root = NULL;
    } else {
      root = root->next;
      root->previous = remNode->previous;
      remNode->previous->next = root;
    }

    if (cursor == remNode) {
      cursor = root;  
    } 

    return remNode;
  }

  /**
     Safely remove the node in the location of the cursor.
   */
  CNode* HandleCursorRemoval(CNode* remNode) {
    remNode = cursor;
    remNode = cursor; 
    cursor = cursor->next;
    
    cursor->previous = remNode->previous;
    remNode->previous->next = cursor;
    return remNode;
  }
public:
  /**
     Constructor for the CirleLinkedList.
   */
  CircleLinkedList(const _Compare& comparator = _Compare(), const _Alloc& allocator = _Alloc() )
    : root(NULL)
    , cursor(NULL)
    , alloc(allocator)
    , _cmp(comparator)
  { 
    this->list_type = interface::CIRCULAR_LINKED_LIST;
    this->size = 0;
  }

  /**
     CircleLinkedList overloaded constructor.
   */
  CircleLinkedList(const _Alloc& allocator )
    : root(NULL)
    , cursor(NULL)
    , alloc(allocator)
  {
    this->list_type = interface::CIRCULAR_LINKED_LIST;
    this->size = 0;
  }

  ~CircleLinkedList() {
    CNode* traverse = root;
    CNode* prev = NULL;
    for (int i = root->index; i < this->getSize(); ++i) {
      prev = traverse;
      traverse = traverse->next;
      delete prev;
      prev = NULL;
    }
  }

  /**
     Insert value into the list.
   */
  void Insert(const_reference value) {
    CNode* newNode = new CNode(*this);
    nullify_pointer(newNode->next);
    nullify_pointer(newNode->previous);
    newNode->AllocateValue(value);

    if ((this->IsEmpty()) || (root == NULL) ) {
      root = newNode;
      root->next = root;
      root->previous = root;
      cursor = root;
    } else {
      newNode->previous = cursor;
      newNode->next = cursor->next;
      cursor->next = newNode;
      newNode->next->previous = newNode;
    }
     
    if (newNode->next != root) {
      int32_t i = newNode->next->index;
      newNode->index = i++;
      CNode* traversal = newNode->next;
      while(traversal != root) {
        traversal->index = i++;
        traversal = traversal->next;
      }
    } else {
      newNode->index = size;
    } 
    this->size++;
  }

  /**
     Remove a value from the list.
   */
  bool Remove(const_reference value) {
    bool result = false;
    if (this->IsEmpty()) {
      return result;
    }

    CNode* remNode; 
    CNode* iteratorNode;
    nullify_pointer(remNode);
    nullify_pointer(iteratorNode);

    if (_cmp.Equal(*root->data, value)) {
      remNode = HandleRootRemoval(remNode);
    } else if (_cmp.Equal(*cursor->data, value)) {
      remNode = HandleCursorRemoval(remNode);
    } else {
      CNode* startNode = cursor;
      cursor = cursor->next;
      while (cursor != startNode) {
	if (_cmp.Equal(*cursor->data, value)) {
	  remNode = HandleCursorRemoval(remNode);
	  break;
	}
	cursor = cursor->next;
      }
    }

    if (remNode != NULL) {
      if ((this->size - 1) > 0) {
        iteratorNode = remNode->next;
        int32_t i = remNode->index;
        iteratorNode->index = i++;
        while (iteratorNode != root) {
	        iteratorNode->index = i++;
	        iteratorNode = iteratorNode->next;
        }
      }

      free_pointer(remNode);
      nullify_pointer(remNode);
      size--;
      result = true;
    }

    return result;
  }

  /**
     Remove a value from the list, according to the specified index within the list.
   */
  bool RemoveAt(const int32_t index) {
    bool result = false;
    if (this->IsEmpty() || 
	(index < 0) ||
	(index >= this->size)) {
      return result;
    }

    CNode* remNode; 
    CNode* iteratorNode;
    nullify_pointer(remNode);
    nullify_pointer(iteratorNode);

    if (index == root->index) {
      remNode = HandleRootRemoval(remNode);
      iteratorNode = root;
    } else if (index == cursor->index) {
      remNode = HandleCursorRemoval(remNode);
      iteratorNode = cursor;
    } else {
      CNode* startNode = cursor;
      if (index > cursor->index) {
	cursor = cursor->next;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    remNode = HandleCursorRemoval(remNode);
	    iteratorNode = cursor;
	    break;	    
	  }
	  cursor = cursor->next;
	}
      } else {
	cursor = cursor->previous;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    remNode = HandleCursorRemoval(remNode);
	    iteratorNode = cursor;
	    break;
	  }
	  cursor = cursor->previous;
	}
      }
    }

    if (remNode != NULL) {
      int32_t i = remNode->index;
      iteratorNode->index = i++;
      while (iteratorNode != root) {
	iteratorNode->index = i++;
	iteratorNode = iteratorNode->next;
      }

      free_pointer(remNode);
      nullify_pointer(remNode);
      size--;
      result = true;
    }
    
    return result;
  }

  /**
     Return the reference of the data in the "Front" of the list.
     this is the root value that will be returned.
   */
  reference Front() {
    return *root->data;
  }
  /**
     Return the previous value that is next to the root. This is the last
     value before wrapping Back to the root.
   */
  reference Back() {
    return *root->previous->data;
  }

  /**
     Return the value reference of the node At the specified index.
   */
  reference At(const int32_t index) {
    if (index == root->index) {
      return *root->data;
    } else if (index == cursor->index) {
      return *cursor->data;
    } else {
      CNode* startNode = cursor;
      if (index > cursor->index) {
	cursor = cursor->next;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    return *cursor->data;
	  }
	  cursor = cursor->next;
	}
      } else {
	cursor = cursor->previous;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    return *cursor->data;
	  }
	  cursor = cursor->previous;
	}
      }
    }
  }
  
  /**
     Check if the value specified is inside this list. Returns true is 
     value is inside this list.
   */
  bool Contains(const_reference value) {
    bool result =false;
    if (_cmp.Equal(value, *root->data)) {
      result = true;
    } else if (_cmp.Equal(value, *cursor->data)) {
      result = true;
    } else {
      CNode* startNode = cursor;
      cursor = cursor->next;
      while (cursor != startNode) {
	if (_cmp.Equal(*cursor->data, value)) {
	  result = true;
	  break;
	}
	cursor = cursor->next;
      }
    }

    return result;
  }

  /**
     Traverses the cursor to the next node.
   */
  void cursorNext() {
    cursor = cursor->next;
  } 

  /**
     Traverses the cursor to the previous node.
   */
  void cursorBack() {
    cursor = cursor->previous;
  } 

  /**
     Returns the value At the cursor node.
   */
  reference getCursor() {
    return *cursor->data;
  } 
private:
  /**
     The main starting node, where the circle linked list begins its link.
   */
  CNode* root;
  /**
     The cursor node, which holds the node that is currently being pointed At by the 
     list. This node traverses the list forwards and backwards, as a means to allow checking 
     the list without having to reset Back At the root, and traverse from there, which will result 
     in linear time, similar to that of single linked list.
   */
  CNode* cursor;

  /**
     Allocator Object used for allocation.
   */
  _Alloc alloc;
  /**
     Comparator object used for comparing object values inside this list.
   */
  _Compare _cmp;
};
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace
#endif // __CIRCLE_LINKED_LIST_H
