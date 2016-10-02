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
#ifndef __QUEUE_H
#define __QUEUE_H

#include <CommProto/tools/data_structures/interface/interface_queue.h>

#include <vcclr.h>
#using <mscorlib.dll>

using namespace System;

namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

#pragma managed

generic<typename Type>
public interface class Queue {
public:

  virtual void EnQueue(Type data) = 0;
  virtual Type DeQueue() = 0;
  virtual Type Front() = 0;
  virtual Type Back() = 0;

  virtual Int32 GetSize() = 0;
  virtual Boolean IsEmpty() = 0;
};
}
}
}
}
#endif // __QUEUE_H