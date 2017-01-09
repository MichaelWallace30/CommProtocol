/*
  Double Linked List data structure.

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
#ifndef __DOUBLE_LINKED_LIST_H
#define __DOUBLE_LINKED_LIST_H

#include <CommProto/tools/data_structures/interface/interface_list.h>
#include <CommProto/tools/standard_comparator.h>
#include <CommProto/tools/allocator/newallocator.h>

#include <CommProto/architecture/macros.h>

#include <string>
#include <exception>

namespace comnet {
namespace tools {
namespace datastructures {

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
  class    _Alloc   = tools::allocator::NewAllocator<_Ty> >
class COMM_EXPORT DoubleLinkedList : public interface::List<_Ty> {
  /*
    TODO(Garcia): We will need to make use of the Allocator.
   */
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef const _Ty const_data_type;
  typedef _Ty& reference_type;

  /**
  Compare object, used to Compare values in the data structure. This can be customized as to
  make this structure more dynamic.
  */
  _Compare cmp;
  /**
  Allocator object, used to Allocate values in the data structure. This can be customized as to make this
  structure more dynamic.
  */
  _Alloc  alloc;
  /**
  Node used as data containers within this data structure.
  */
  class DNode {
  public:
    DoubleLinkedList<_Ty, _Compare, _Alloc>& ref;
    DNode* next;
    DNode* previous;

    _Ty* data;
    int32_t index;

    DNode(DoubleLinkedList<_Ty, _Compare, _Alloc>& r) 
      : data(NULL)
      , index(0)
      , next(NULL)
      , previous(NULL)
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

    ~DNode() {
      DeallocateValue();
    }
  };
public:
  /**
     Default Constructor for data structure. 
   */
  DoubleLinkedList(const _Compare& comparator = _Compare(),
     const _Alloc& allocator = _Alloc() )
  : root(NULL)
  , tail(NULL)
  , cursor(NULL)
  , alloc(allocator) 
  , cmp(comparator)
  {
    this->list_type = interface::DOUBLE_LINKED_LIST;
    this->size = 0;
  }

  DoubleLinkedList(const _Alloc& allocator ) 
    : alloc(allocator)
    , root(NULL)
    , tail(NULL)
    , cursor(NULL)
  {
    this->list_type = interface::DOUBLE_LINKED_LIST;
    this->size = 0;
  }

  /**
     Default destructor for the data structure.
   */
  virtual ~DoubleLinkedList() {
    _delete_list(DNode, this->size);
    nullify_pointer(root);
    nullify_pointer(tail);
    nullify_pointer(cursor);
  }
  /**
     Inserts a value into the data structure.
   */
  virtual void Insert(const_reference value) {
    DNode* new_node = new DNode(*this);
    nullify_pointer(new_node->next);
    nullify_pointer(new_node->previous);
    new_node->AllocateValue(value);

    if (this->size <= 0) {
      root = tail = cursor = new_node;
    } else {
      tail->next = new_node;
      new_node->previous = tail;
      tail = new_node;
    }

    new_node->index = this->size++;
  }

private:
  
  /**
     Handles the setup to remove the root value.
   */
  DNode* HandleRootRemoval(DNode* rem_node) {
    rem_node = root;
    root = root->next;
    
    if (cursor == rem_node) {
      cursor = root;
    }

    if (tail == rem_node) {
      tail = tail->next;
    }
    
    if (root != NULL) {
      nullify_pointer(root->previous);
    }
    return rem_node;
  }
  /**
     Handles the setup to remove the tail value.
   */
  DNode* HandleTailRemoval(DNode* rem_node) {
    rem_node = tail;
    tail = tail->previous;

    if (cursor == rem_node) {
      cursor = cursor->previous;
      nullify_pointer(cursor->next);
    }

    if (root == rem_node) {
      root = root->previous;
      nullify_pointer(root->next);
    }

    nullify_pointer(tail->next);
    return rem_node;
  }

  /**
     Handles the setup to remove the cursor.
   */
  DNode* HandleCursorRemoval(DNode* rem_node) {
    rem_node = cursor;
    
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
      cursor->previous = rem_node->previous;
      rem_node->previous->next = cursor;
    }
    
    return rem_node;
  }

  /**
     Handles the setup to remove the selected node. Indices are reorganized so 
     as to keep the stability of the index for each remaining nodes in the data structure.
   */
  inline
  void HandleRemNodeDelete(DNode* rem_node) {
    DNode* traverse = rem_node->next;
    for (int i = rem_node->index; i < this->size - 1; ++i) {
      traverse->index = i;
      traverse = traverse->next;
    }
    
    free_pointer(rem_node);
  }
  

public:
  /**
     Removes a node based on the specified value. This may be a greedy method, considering it will not
     remove all nodes similar to specified value.
   */
  virtual bool Remove(const_reference value) {
    bool success = false;
    if (this->IsEmpty()) {
      return success;
    }

    DNode* rem_node = NULL;
    
    if (cmp.Equal(*root->data, value)) {
      rem_node = HandleRootRemoval(rem_node);
    } else if (cmp.Equal(*tail->data , value)) {
      rem_node = HandleTailRemoval(rem_node);
    } else if (cmp.Equal(*cursor->data, value)) {
      rem_node = HandleCursorRemoval(rem_node);
    } else {
      cursor = root->next;
      
      while (cursor != tail && cursor != NULL) {
 if (cmp.Equal(*cursor->data , value)) {
   rem_node = cursor;
   cursor = cursor->next;
   
   cursor->previous = rem_node->previous;
   rem_node->previous->next = cursor;
   break;
 }
 
 cursor = cursor->next;
      }
    }

    if (rem_node != NULL) {
      HandleRemNodeDelete(rem_node);
      this->size--;
      success = true;
    }

    return success;
  }

  /**
     Remove a node from the list on the specified index.
   */
  bool RemoveAt(const int32_t index) override {
    bool success = false;
    if (index >= this->size || index < 0) {
      return success;
    }

    DNode* rem_node;
    nullify_pointer(rem_node);
    
    if (index == root->index) {
      rem_node = HandleRootRemoval(rem_node);
    } else if (index == tail->index) {
      rem_node = HandleTailRemoval(rem_node);
    } else if (index == cursor->index) {
      rem_node = HandleCursorRemoval(rem_node);
    } else {
      if (cursor->index > index) {
 cursor = cursor->previous;
 while (cursor != NULL) {
   if (cursor->index == index) {
     rem_node = cursor;
     cursor = cursor->next;
     
     cursor->previous = rem_node->previous;
     rem_node->previous->next = cursor;
     break;
   }
   cursor->previous;
 }
      } else {
 cursor = cursor->next;
 while (cursor != NULL) {
   if (cursor->index == index) {
     rem_node = cursor;
     cursor = cursor->next;
     
     cursor->previous = rem_node->previous;
     rem_node->previous->next = cursor;
     break;
   }
   cursor = cursor->next;
 }
      }
    }

    if (rem_node != NULL) {
      HandleRemNodeDelete(rem_node);
      this->size--;
      success = true;
    }
    
    return success;
  }

  /**
     Get the Front root value of this data structure.
   */
  virtual reference_type Front() {
    return *root->data;
  }
  /**
     Get the Back, tail, value of this data structure.
   */
  virtual reference_type Back() { 
    return *tail->data;
  }

  virtual reference_type At(const int32_t index) {
    if (index >= this->GetSize() || index < 0) {
      throw std::range_error("segmentation fault: access index: " + std::to_string(index));
    }
    if (root->index == index) {
      return *root->data;
    } else if (tail->index == index) {
      return *tail->data;
    } else if (cursor->index == index) {
      return *cursor->data;
    } else {
      if (cursor->index > index) {
        cursor = cursor->previous;
        while (cursor != NULL) {
          if (cursor->index == index) {
            return *cursor->data;
          } else {
            cursor = cursor->previous;
          }
        }
      } else {
        cursor = cursor->next;
        while (cursor != NULL) {
          if (cursor->index == index) {
            return *cursor->data;
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
  virtual bool Contains(const_reference value) {
    bool success = false;
    
    if (cmp.Equal(*root->data, value) ||
 cmp.Equal(*tail->data, value) ||
 cmp.Equal(*cursor->data, value)) {
      success = true;
    } else {
      cursor = root->next;
      while (cursor != NULL) {
 if (cmp.Equal(*cursor->data, value)) {
   success = true;
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
  virtual const _Ty& GetCurrent() {
    return *cursor->data;
  }
protected:
  /**
     Root node, the Front, head, of the data structure.
   */
  DNode* root;
  /**
     Tail node, the Back, tail, of the data structure.
   */
  DNode* tail;
  /**
     Cursor node, the current node in the data structure.
   */
  DNode* cursor;
};
} // DataStructures namespace 
} // Tools namespace
} // Comnet namespace
#endif // __DOUBLE_LINKED_LIST_H
