/*
  Single Linked List data structure.

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
#ifndef __SINGLE_LINKED_LIST_H
#define __SINGLE_LINKED_LIST_H

#include <tools/data_structures/interface/InterfaceList.h>
#include <tools/StandardComparator.h>
#include <architecture/macros.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   Single Linked List provides a basic data structure list to hold all values efficiently.
   It provides O(1) inserstion, and fast removal of certain values. Best used when keeping
   Track of a short list of objects.
*/
template<typename _Ty, 
         class _Compare = StandardComparator<_Ty> >
class SingleLinkedList : public Interface::List< _Ty > {
public:
  /**
     Constructor for the SingleLinkedList data structure.
  */
  SingleLinkedList() 
    : root(NULL)
    , tail(NULL) { 
    this->listType = Interface::SINGLE_LINKED_LIST;
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
  void insert(_Ty value) {
    SNode* newNode = new SNode();
    nullify_pointer(newNode->next);
    newNode->value = value;

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
  bool remove(const _Ty& value) {
    if (this->isEmpty()) {
      return false;
    }
    bool result = false;
    SNode* remNode;
    nullify_pointer(remNode);
    
    if (_cmp.equal(value, root->value)) {
      remNode = handleRootRemoval();
    } else if (_cmp.equal(value, tail->value)) {
      remNode = handleTailRemoval();
    } else {
      SNode* traverse = root->next;
      SNode* previous = root;
      
      while (traverse != NULL) {
	if (_cmp.equal(traverse->value, value)) {
	  previous->next = traverse->next;
	  remNode = traverse;
	  break;
	}
	
	previous = traverse;
	traverse = traverse->next;
      }
    }
    
    if (remNode != NULL) {
      SNode* traverse = remNode->next;
      for (int32_t i = remNode->index; i < this->size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete remNode;
      nullify_pointer(remNode);
      this->size--;
      result = true;
    }

    return result;
  }

  /**
     Removes a value from the list at the specified index. Will return false if the 
     index goes over the size limit or below zero.
  */
  bool removeAt(const int32_t index) {
    if (index < 0 || index >= this->size) {
      return false;
    }

    bool result = false;
    SNode* remNode;
    nullify_pointer(remNode);

    if (root->index == index) {
      remNode = handleRootRemoval();
    } else if (tail->index == index) {
      remNode = handleTailRemoval();
    } else {
      SNode* previous = root;
      SNode* traverse = root->next;
      
      while (traverse != NULL) {
	if (traverse->index == index) {
	  previous->next = traverse->next;
	  remNode = traverse;
	  break;
	}
	  
	previous = traverse;
	traverse = traverse->next;
      }
    }
    
    if (remNode != NULL) {
      SNode* traverse = remNode->next;
      for (int32_t i = remNode->index; i < this->size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete remNode;
      nullify_pointer(remNode);
      this->size--;
      result = true;
    } 

    return result;
  }

  /**
     Get the value at the specified index.
  */
  _Ty& at(const int32_t index) {
    SNode* traverse = root;
    while (traverse->index != index) {
      traverse = traverse->next;
    }
    return traverse->value;
  }

  /**
     Check if this list contains the value specified.
  */
  bool contains(const _Ty& value) {
    bool result = false;
    
    if (_cmp.equal(root->value, value)) {
      result = true;
    } else if (_cmp.equal(tail->value, value)) {
      result = true;
    } else {
      SNode* traverse = root->next;
      
      while (traverse != tail) {
	if (_cmp.equal(traverse->value, value)) {
	  result = true;
	  break;
	}
	
	traverse = traverse->next;
      }
    }

    return result;
  }
  
  /**
     Return the front most value of the list.
  */
  _Ty& front() {
    return root->value;
  }

  /**
     Return the last value that was added.
  */
  _Ty& back() {
    return tail->value;
  }

private:
  /**
     Secretive node used for this List.
  */
  struct SNode {
    SNode* next;
    
    _Ty value;
    
    int32_t index;
  };

  /** 
      Our root node.
  */
  SNode* root;
  /**
     Our tail node, which will be set at the last node of this list.
  */
  SNode* tail;
  /**
     Compare function.
  */
  _Compare _cmp;

  /**
     Handles the removal of the root node.
  */
  SNode* handleRootRemoval() {
    SNode* remNode = root;
    
    if (root != NULL) {
      root = root->next;
    }
    return remNode;
  }
  /** 
      Handles the removal of the tail node.
  */
  SNode* handleTailRemoval() {
    SNode* remNode = tail;
    SNode* traverse = root;
    
    while (traverse->next != tail) {
      traverse = traverse->next;
    }
    tail = traverse;
    // Nullify the new tail's next value, since we are deleting it.

    nullify_pointer(tail->next);
    return remNode;
  }
}; 
} // DataStructure
} // Tools
} // Comnet 

#endif // __SINGLE_LINKED_LIST_H
