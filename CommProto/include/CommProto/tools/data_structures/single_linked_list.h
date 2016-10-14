/*
  Single Linked List data structure.

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
#ifndef __SINGLE_LINKED_LIST_H
#define __SINGLE_LINKED_LIST_H

#include <CommProto/tools/data_structures/interface/interface_list.h>
#include <CommProto/tools/allocator/newallocator.h>
#include <CommProto/tools/standard_comparator.h>
#include <CommProto/architecture/macros.h>

namespace comnet {
namespace tools {
namespace datastructures {

/**
   Single Linked List provides a basic data structure list to hold all values efficiently.
   It provides O(1) inserstion, and fast removal of certain values. Best used when keeping
   Track of a short list of objects.
*/
_COMNET_PUBLIC_API_
template<typename _Ty, 
         class    _Compare = StandardComparator<_Ty>,
	 class    _Alloc   = tools::allocator::NewAllocator<_Ty> >
class COMM_EXPORT SingleLinkedList : public interface::List< _Ty > {
public:
  /**
     Constructor for the SingleLinkedList data structure.
  */
  SingleLinkedList(const _Compare& comparator = _Compare(), 
		   const _Alloc& allocator = _Alloc() ) 
    : root(NULL)
    , tail(NULL)
    , alloc(allocator)
    , _cmp(comparator)  
  { 
    this->list_type = interface::SINGLE_LINKED_LIST;
    this->size = 0;
  }
  
  SingleLinkedList(const _Alloc& allocator ) 
    : alloc(allocator)
    , root(NULL)
    , tail(NULL)
  {
    this->list_type = interface::SINGLE_LINKED_LIST;
    this->size = 0;
  }

  /**
     Single Linked List destructor.
   */
  ~SingleLinkedList() {
    _delete_list(SNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
  }

  /**
     Insert a value into the list, this will likely copy over the value
     and set it into the data structure.
  */
  void Insert(const _Ty& value) {
    SNode* newNode = new SNode(*this);
    nullify_pointer(newNode->next);
    newNode->AllocateValue(value);

    if ((root == NULL) && (this->size <= 0)) {
      root = tail = newNode;
      newNode->index = this->size++;
    } else {
      tail->next = newNode;
      tail = tail->next;
      newNode->index = this->size++;
    }
  }

  /**
     Remove a value from the list. Returns true if the list has removed the specified
     value, false otherwise.
  */
  bool Remove(const _Ty& value) {
    if (this->IsEmpty()) {
      return false;
    }
    bool result = false;
    SNode* rem_node;
    nullify_pointer(rem_node);
    
    if (_cmp.Equal(value, *root->value)) {
      rem_node = HandleRootRemoval();
    } else if (_cmp.Equal(value, *tail->value)) {
      rem_node = HandleTailRemoval();
    } else {
      SNode* traverse = root->next;
      SNode* previous = root;
      
      while (traverse != NULL) {
	if (_cmp.Equal(*traverse->value, value)) {
	  previous->next = traverse->next;
	  rem_node = traverse;
	  break;
	}
	
	previous = traverse;
	traverse = traverse->next;
      }
    }
    
    if (rem_node != NULL) {
      SNode* traverse = rem_node->next;
      for (int32_t i = rem_node->index; i < this->size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete rem_node;
      nullify_pointer(rem_node);
      this->size--;
      result = true;
    }

    return result;
  }

  /**
     Removes a value from the list At the specified index. Will return false if the 
     index goes over the size limit or below zero.
  */
  bool RemoveAt(const int32_t index) {
    if (index < 0 || index >= this->size) {
      return false;
    }

    bool result = false;
    SNode* rem_node;
    nullify_pointer(rem_node);

    if (root->index == index) {
      rem_node = HandleRootRemoval();
    } else if (tail->index == index) {
      rem_node = HandleTailRemoval();
    } else {
      SNode* previous = root;
      SNode* traverse = root->next;
      
      while (traverse != NULL) {
	if (traverse->index == index) {
	  previous->next = traverse->next;
	  rem_node = traverse;
	  break;
	}
	  
	previous = traverse;
	traverse = traverse->next;
      }
    }
    
    if (rem_node != NULL) {
      SNode* traverse = rem_node->next;
      for (int32_t i = rem_node->index; i < this->size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete rem_node;
      nullify_pointer(rem_node);
      this->size--;
      result = true;
    } 

    return result;
  }

  /**
     Get the value At the specified index.
  */
  _Ty& At(const int32_t index) {
    SNode* traverse = root;
    while (traverse->index != index) {
      traverse = traverse->next;
    }
    return *traverse->value;
  }

  /**
     Check if this list Contains the value specified.
  */
  bool Contains(const _Ty& value) {
    bool result = false;
    
    if (_cmp.Equal(*root->value, value) 
	|| _cmp.Equal(*tail->value, value)) {
      result = true;
    } else {
      SNode* traverse = root->next;
      
      while (traverse != tail) {
	if (_cmp.Equal(*traverse->value, value)) {
	  result = true;
	  break;
	}
	
	traverse = traverse->next;
      }
    }

    return result;
  }
  
  /**
     Return the Front most value of the list.
  */
  _Ty& Front() {
    return *root->value;
  }

  /**
     Return the last value that was added.
  */
  _Ty& Back() {
    return *tail->value;
  }

private:
  /**
     Secretive node used for this List.
  */
  class SNode {
  public:
    SingleLinkedList<_Ty, _Compare, _Alloc>& ref;
    SNode* next;
    
    _Ty* value;
    
    int32_t index;

    SNode(SingleLinkedList<_Ty, _Compare, _Alloc>& r) 
      : value(NULL)
      , next(NULL)
      , index(0)
      , ref(r)
    { }

    void AllocateValue(const _Ty& value) {
      this->value = ref.alloc.Allocate(0);
      ref.alloc.Construct(this->value, value);
    }

    void DeallocateValue() {
      ref.alloc.Destruct(value);
      ref.alloc.Deallocate(value);
    }

    ~SNode() {
      DeallocateValue();
    }
  };

  /** 
      Our root node.
  */
  SNode* root;
  /**
     Our tail node, which will be set At the last node of this list.
  */
  SNode* tail;
  /**
     Compare function.
  */
  _Compare _cmp;

  /**
     Special allocator unit.
   */
  _Alloc alloc;

  /**
     Handles the removal of the root node.
  */
  SNode* HandleRootRemoval() {
    SNode* rem_node = root;
    
    if (root != NULL) {
      root = root->next;
    }
    return rem_node;
  }
  /** 
      Handles the removal of the tail node.
  */
  SNode* HandleTailRemoval() {
    SNode* remnode = tail;
    SNode* traverse = root;
    
    while (traverse->next != tail) {
      traverse = traverse->next;
    }
    tail = traverse;
    // Nullify the new tail's next value, since we are deleting it.

    nullify_pointer(tail->next);
    return remnode;
  }
}; 
} // DataStructure
} // Tools
} // Comnet 

#endif // __SINGLE_LINKED_LIST_H
