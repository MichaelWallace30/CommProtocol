/*
Copyright (C) 2016  Michael Wallace, Mario Garcia.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __CAUTO_QUEUE_H
#define __CAUTO_QUEUE_H

#include <tools/data_structures/interface/Queue.h>

#include <vcclr.h>
#using <mscorlib.dll>


namespace Comnet {
namespace Tools {
namespace DataStructures {


using namespace System;
using namespace System::Collections;
using namespace Comnet::Tools::DataStructures::Interface;


/**
  Automatic queue data structure implemented for the use of Customization.
 */
generic<typename Type>
public ref class AutoQueue : public Queue<Type> {
public:
  AutoQueue();
  
  virtual void EnQueue(Type data);
  virtual Type DeQueue();
  virtual Type Front();
  virtual Type Back();

  virtual Int32 GetSize();
  virtual Boolean IsEmpty();
private:
  Collections::Queue^ managedQueue;
};
}
}
}
#endif // __CAUTO_QUEUE_H