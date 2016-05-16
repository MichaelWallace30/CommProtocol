#ifndef __SINGLE_LINKED_LIST_H
#define __SINGLE_LINKED_LIST_H

#include <tools/data_structures/interface/InterfaceList.h>
#include <tools/Comparator.h>

namespace Comnet {
namespace DataStructure {

template<typename _Ty, 
	 class _Compare = Comparator<_Ty>>
class SingleLinkedList : public List<_Ty> {
private:
  
  struct SNode {
    SNode* next;
    _Ty value;
  };

  SNode* root;
  SNode* tail;

  _Compare _cmp;

public:
  SingleLinkedList() : root(NULL) { 
    listType = SINGLE_LINKED_LIST;
  }

  void insert(_Ty value) {
    SNode* newNode = new SNode();
    newNode.next = NULL;
    newNode.value = value;

    if ((root == NULL) && (size <= 0)) {
      root = newNode;
    } else {
      SNode* traverse = root;
      
      while (traverse->next != NULL) {
	traverse = traverse->next;
      }
      traverse->next = newNode;
    }
  }

  const _Ty& remove(const _Ty& value) {
    
  }
  
  
}; 
} // DataStructure
} // Comnet 

#endif // __SINGLE_LINKED_LIST_H
