#ifndef __AUTO_QUEUE_H
#define __AUTO_QUEUE_H

#include <tools/data_structures/interface/InterfaceQueue.h>

#include <queue>

namespace Comnet {
namespace DataStructure {

/**
   AutoQueue is a wrapper implementation of the Standard Template
   Library's queue. The reason we are wrapping it is so that we can
   control the way it is used in our library so as to make it much
   easier to use.
*/
template<typename _Ty>
class AutoQueue : public InterfaceQueue<_Ty> {
public:
  /**
     Constructor for AutoQueue.
  */
  AutoQueue() { queueType = AUTO_QUEUE; }
  /**
     Destructor for AutoQueue.
  */
  ~AutotQueue() { }
  /**
     Grab the size of this queue.
  */
  int32_t getsize() { return queue.size(); }
  /**
     Insert a value into the back of this queue.
  */
  void enQueue(const _Ty& data) { queue.push(data); }
  /**
     Remove a value from front of the queue.
  */
  void deQueue() { queue.pop(); }
  /**
     Check the value infront of the queue.
  */
  const _Ty& front() { return queue.front(); }
  /**
     Check the value in back of the queue.
  */
  const _Ty& back() { return queue.back(); }
  /**
     Check if this queue is empty.
  */
  bool isEmpty() { return queue.empty(); }
  /**
     Check if queue is empty. Similar to isEmpty().
  */
  bool empty() { return isEmpty(); }
  /**
     Push a value into the front of the queue. Similar to enQueue().
  */
  void push(const _Ty& data) { queue.push(data); }
  /**
     Pop a value from the front of the queue. Similar to deQueue().
  */
  void pop() { queue.pop(); }
private:
  /** 
      Standard Template Library Queue, which this Class AutoQueue is wrapping over.
  */
  std::queue<_Ty> queue;
};
} // DataStructure
} // Comnet

#endif // __AUTO_QUEUE_H
