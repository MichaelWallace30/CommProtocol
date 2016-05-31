#ifndef __LINKED_QUEUE_H
#define __LINKED_QUEUE_H

#include <tools/data_structures/interface/InterfaceQueue.h>
#include <tools/data_structures/SingleLinkedList.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/* */
template<typename _Ty
	 class _Compare>
class SingleLinkedQueue : public SingleLinkedList<_Ty, _Compare>  { 
public:
  SingleLinkedQueue() { 
  }
private:
  
};

} // DataStructures
} // Tools
} // Comnet
#endif // __LINKED_QUEUE_H
