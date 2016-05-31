#ifndef __INTERFACE_QUEUE_H
#define __INTERFACE_QUEUE_H

#include <tools/data_structures/interface/Generic.h>

using namespace Comnet::Tools::DataStructures::Interface;

namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

/**
   QueueType holds the value of what ever the generic queue may be.
*/
enum QueueType {
  SINGLE_LINKED_QUEUE,
  DOUBLE_LINKED_QUEUE,
  PRIORITY_QUEUE,
  AUTO_QUEUE,
}
  
/** 
    Queue interface that must be inherited by all queue data structures. 
*/
template<typename _Ty>
class Queue : public Generic {
public:
  /**
     Queue Constructor.
  */
  Queue() { type = QUEUE; }
  /**
     Polymorphc Queue destructor.
  */
  virtual ~Queue()  { } 
  /**
     Get the size of this queue.
  */
  virtual int32_t getSize() { return size; }
  /**
     get the data structure type.
  */
  virtual DataStructureType getDataStructType() { return type; }
  /**
     Get the queue type.
  */
  virtual QueueType getQueueType() { return queueType; }
  /**
     Add a value into the queue.
  */
  virtual void enQueue(const _Ty& data) = 0;
  /**
     Remove a value from the queue.
  */
  virtual void deQueue() = 0;
  /**
     Check the front of this queue.
  */
  virtual const _Ty& front() = 0;
  /**
     Check the back of this queue.
  */
  virtual const _Ty& back() = 0;
private:
  /**
     Holds the queue type value.
  */
  QueueType queueType;
};
} // Interface
} // DataStructures
} // Tools
} // Comnet
#endif // __INTERFACE_QUEUE_H
