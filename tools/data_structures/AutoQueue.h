#ifndef __AUTO_QUEUE_H
#define __AUTO_QUEUE_H

#include <tools/data_structures/interface/InterfaceQueue.h>

#include <queue>

namespace Comnet {
namespace DataStructure {

template<typename _Ty>
class AutoQueue : public InterfaceQueue<_Ty> {
public:
  AutoQueue() { queueType = AUTO_QUEUE; }
  ~AutotQueue() { }
  int32_t getsize() { return queue.size(); }
  
  void enQueue(const _Ty& data) { queue.push(data); }
  void deQueue() { queue.pop(); }

  const _Ty& front() { return queue.front(); }
  const _Ty& back() { return queue.back(); }
  
  bool isEmpty() { return queue.empty(); }
  bool empty() { return isEmpty(); }

  void push(const _Ty& data) { queue.push(data); }
  void pop() { queue.pop(); }
private:
  std::queue<_Ty> queue;
};
} // DataStructure
} // Comnet

#endif // __AUTO_QUEUE_H
