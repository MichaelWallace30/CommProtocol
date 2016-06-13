/*
  Circle linked list.

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
#ifndef __CIRCLE_LINKED_LIST_H
#define __CIRCLE_LINKED_LIST_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
#include <CommProto/tools/StandardComparator.h>
#include <CommProto/tools/allocator/NewAllocator.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   Circle linked list acts like a double linked list, except the fact that the tail
   references back to the head of the list. This is a good data structure to use for 
   holding information in which either need to be constantly looked over, allowing the 
   program to switch between processes or threads, allowing the user to move between different
   programs and much more.
 */
template<typename _Ty,
	 typename _Compare = StandardComparator<_Ty>,
	 typename _Alloc   = Allocator::NewAllocator<_Ty> >
class CircleLinkedList : public Interface::List<_Ty> {
  typedef _Ty* pointer;
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;

  struct CNode {
    CNode* next;
    CNode* previous;
    
    _Ty data;
    int32_t index;
  };

  CNode* handleRootRemoval(CNode* remNode) {
    CNode* next = root->next;
    CNode* previous = root->previous;
    remNode = root;

    if ((root->next == root) &&
	(root->previous == root)) {
      root = NULL;
    } else {
      root = root->next;
    }

    return remNode;
  }

  CNode* handleCursorRemoval(CNode* remNode) {
    remNode = cursor;
    remNode = cursor; 
    cursor = cursor->next;
    
    cursor->previous = remNode->previous;
    remNode->previous->next = cursor;
    return remNode;
  }
public:
  CircleLinkedList(const _Compare& comparator = _Compare(), const _Alloc& allocator = _Alloc() )
    : root(NULL)
    , cursor(NULL)
    , alloc(allocator)
    , _cmp(comparator)
  { 
    this->listType = Interface::CIRCULAR_LINKED_LIST;
    this->size = 0;
  }

  CircleLinkedList(const _Alloc& allocator = _Alloc() )
    : root(NULL)
    , cursor(NULL)
    , alloc(allocator)
  {
    this->listType = Interface::CIRCLAR_LINKED_LIST;
    this->size = 0;
  }

  void insert(const_reference value) {
    CNode* newNode = allocate_pointer(CNode);
    nullify_pointer(newNode->next);
    nullify_pointer(newNode->previous);
    newNode->data = value;

    if ((this->isEmpty()) || (root == NULL) ) {
      root = newNode;
      root->next = root;
      root->previous = root;
      cursor = root;
    } else {
      newNode->prev = cursor;
      newNode->next = cursor->next;
      cursor->next = newNode;
      newNode->next->prev = newNode;
    }

    newNode->index = this->size++;
  }

  bool remove(const_reference value) {
    bool result = false;
    if (this->isEmpty()) {
      return result;
    }

    CNode* remNode, iteratorNode;
    nullify_pointer(remNode);
    nullify_pointer(iteratorNode);

    if (_cmp.equal(root->data, value)) {
      remNode = handleRootRemoval(remNode);
      iteratorNode = root;
    } else if (_cmp.equal(cursor->data, value)) {
      remNode = handleCursorRemoval(remNode);
      iteratorNode = cursor;
    } else {
      CNode* startNode = cursor;
      cursor = cursor->next;
      while (cursor != startNode) {
	if (_cmp.equal(cursor->data, value)) {
	  remNode = handleCursorRemoval(remNode);
	  iteratorNode = cursor;
	  break;
	}
	cursor = cursor->next;
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

  bool removeAt(const int32_t index) {
    bool result = false;
    if (this->isEmpty() || 
	(index < 0) ||
	(index >= this->size)) {
      return result;
    }

    CNode* remNode, iteratorNode;
    nullify_pointer(remNode);
    nullify_pointer(iteratorNode);

    if (index == root->index) {
      remNode = handleRootRemove(remNode);
      iteratorNode = root;
    } else if (index == cursor->index) {
      remNode = handleCursorRemove(remNode);
      iteratorNode = cursor;
    } else {
      CNode* startNode = cursor;
      if (index > cursor->index) {
	cursor = cursor->next;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    remNode = handleCursorRemoval(remNode);
	    iteratorNode = cursor;
	    break;	    
	  }
	  cursor = cursor->next;
	}
      } else {
	cursor = cursor->previous;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    remNode = handleCursorRemoval(remNode);
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

  reference front() {
    return root->data;
  }

  reference back() {
    return root->previous->data;
  }

  reference at(const int32_t index) {
    if (index == root->index) {
      return root->data;
    } else if (index == cursor->index) {
      return cursor->data;
    } else {
      CNode* startNode = cursor;
      if (index > cursor->index) {
	cursor = cursor->next;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    return cursor->data;
	  }
	  cursor = cursor->next;
	}
      } else {
	cursor = cursor->previous;
	while (cursor != startNode) {
	  if (index == cursor->index) {
	    return cursor->data;
	  }
	  cursor = cursor->previous;
	}
      }
    }
  }
  
  bool contains(const_reference value) {
    bool result =false;
    if (_cmp.equal(value, root->data)) {
      result = true;
    } else if (_cmp.equal(value, cursor, cursor->data)) {
      result = true;
    } else {
      CNode* startNode = cursor;
      cursor = cursor->next;
      while (cursor != startNode) {
	if (_cmp.equal(cursor->data, value)) {
	  result = true;
	  break;
	}
	cursor = cursor->next;
      }
    }

    return result;
  }
private:
  CNode* root;
  CNode* cursor;

  _Alloc alloc;
  _Compare _cmp;
};
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace
#endif // __CIRCLE_LINKED_LIST_H
