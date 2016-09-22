#include <tools/data_structures/CAutoQueue.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {


generic<typename Type>
CAutoQueue<Type>::CAutoQueue()
: managedQueue(gcnew Collections::Queue())
{
}


generic<typename Type>
void CAutoQueue<Type>::EnQueue(Type data) {
  managedQueue->Enqueue(data);
}


generic<typename Type>
Type CAutoQueue<Type>::DeQueue() {
  return (Type)managedQueue->Dequeue();
}


generic<typename Type>
Type CAutoQueue<Type>::Front() {
  return (Type)managedQueue->Peek();
}


generic<typename Type>
Type CAutoQueue<Type>::Back() {
  throw gcnew System::NullReferenceException("Cannot reference back of queue pointer for Collections.");
}


generic<typename Type>
Int32 CAutoQueue<Type>::GetSize() {
  return managedQueue->Count;
}


generic<typename Type>
Boolean CAutoQueue<Type>::IsEmpty() {
  return (managedQueue->Count == 0);
}
}
}
}