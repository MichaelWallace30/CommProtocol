#ifndef __INTERFACE_QUEUE_H
#define __INTERFACE_QUEUE_H

#include <tools/data_structures/interface/Generic.h>

using namespace Comnet::DataStructure::Package;

namespace Comnet {
namespace DataStructure {

enum QueueType {
  SINGLE_LINKED_QUEUE,
  DOUBLE_LINKED_QUEUE,
  PRIORITY_QUEUE,
  AUTO_QUEUE,
  QUEUE,
}
  
/** Queue interface */
template<typename _Ty>
class Queue : public Interface {
public:
  Queue();
  virtual ~Queue();

  virtual int32_t getSize() { return size; }
  virtual DataStructureType getDataStructType() { return type; }
  virtual QueueType getQueueType() { return queueType; }
  
  virtual void enQueue(const _Ty& data) = 0;
  virtual void deQueue() = 0;
  virtual const _Ty& front() = 0;
private:
  QueueType queueType;
};
} // DataStructure
} // Comnet
#endif // __INTERFACE_QUEUE_H
