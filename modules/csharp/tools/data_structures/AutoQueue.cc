#include <tools/data_structures/AutoQueue.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {


generic<typename Type>
AutoQueue<Type>::AutoQueue()
: managedQueue(gcnew Collections::Queue())
{
}


generic<typename Type>
void AutoQueue<Type>::EnQueue(Type data) {
  managedQueue->Enqueue(data);
}


generic<typename Type>
Type AutoQueue<Type>::DeQueue() {
  return (Type)managedQueue->Dequeue();
}


generic<typename Type>
Type AutoQueue<Type>::Front() {
  return (Type)managedQueue->Peek();
}


generic<typename Type>
Type AutoQueue<Type>::Back() {
  throw gcnew System::NullReferenceException("Cannot reference back of queue pointer for Collections.");
}


generic<typename Type>
Int32 AutoQueue<Type>::GetSize() {
  return managedQueue->Count;
}


generic<typename Type>
Boolean AutoQueue<Type>::IsEmpty() {
  return (managedQueue->Count == 0);
}
}
}
}