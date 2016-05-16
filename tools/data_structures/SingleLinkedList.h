#ifndef __SINGLE_LINKED_LIST_H
#define __SINGLE_LINKED_LIST_H

#include <tools/data_structures/interface/InterfaceList.h>
#include <tools/Comparator.h>

#define NULL 0

namespace Comnet {
namespace DataStructure {

template<typename _Ty, class _Compare = Comparator<_Ty> >
class SingleLinkedList : public List< _Ty > {
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
    tail->next = NULL;

    return remNode;
  }

public:
  /**
     Constructor for the SingleLinkedList data structure.
  */
  SingleLinkedList() : root(NULL) { 
    List<_Ty>::listType = SINGLE_LINKED_LIST;
    Interface::size = 0;
  }

  /**
     Insert a value into the list, this will likely copy over the value
     and set it into the data structure.
  */
  void insert(_Ty value) {
    SNode* newNode = new SNode();
    newNode->next = NULL;
    newNode->value = value;

    if ((root == NULL) && (Interface::size <= 0)) {
      root = tail = newNode;
      newNode->index = Interface::size++;
    } else {
      tail->next = newNode;
      tail = tail->next;
      newNode->index = Interface::size++;
    }
  }

  /**
     Remove a value from the list. Returns true if the list has removed the specified
     value, false otherwise.
  */
  bool remove(const _Ty& value) {
    if (List<_Ty>::isEmpty()) {
      return false;
    }
    bool result = false;
    SNode* remNode = NULL;
    
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
      for (int32_t i = remNode->index; i < Interface::size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete remNode;
      remNode = NULL;
      Interface::size--;
      result = true;
    }

    return result;
  }

  /**
     Removes a value from the list at the specified index. Will return false if the 
     index goes over the size limit or below zero.
  */
  bool removeAt(const int32_t index) {
    if (index < 0 || index >= Interface::size) {
      return false;
    }

    bool result = false;
    SNode* remNode = NULL;

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
      for (int32_t i = remNode->index; i < Interface::size - 1; ++i) {
	traverse->index = i;
	traverse = traverse->next;
      }
      
      delete remNode;
      remNode = NULL;
      Interface::size--;
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
}; 
} // DataStructure
} // Comnet 

#endif // __SINGLE_LINKED_LIST_H
