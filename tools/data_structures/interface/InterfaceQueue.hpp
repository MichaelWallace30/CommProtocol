#ifndef __INTERFACE_QUEUE_H
#define __INTERFACE_QUEUE_H

#include <CommProtocol/tools/data_structures/interface/Generic.hpp>

using namespace Comnet::DataStructure::Package;

namespace Comnet {
namespace DataStructure {

enum QueueType {
  SINGLE_LINKED_QUEUE,
  DOUBLE_LINKED_QUEUE,
  PRIORITY_QUEUE,
  QUEUE,
}
  
/** Queue interface */
template<typename T>
class Queue : public Interface {
public:
  Queue();
  virtual ~Queue();

  int32_t getSize() { return size; }
  DataStructureType getDataStructType() { return type; }
  QueueType getQueueType() { return queueType; }
  
  virtual void enQueue(T data) = 0;
  virtual void deQueue() = 0;
  virtual T& front() = 0;
private:
  QueueType queueType;
};
} // DataStructure
} // Comnet
#endif // __INTERFACE_QUEUE_H
