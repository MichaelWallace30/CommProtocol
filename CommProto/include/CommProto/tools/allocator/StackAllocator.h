/*
  Stack method  allocator.

  Copyright (C) 2016  Mario Garcia.

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
#ifndef __STACK_ALLOCATOR_H
#define __STACK_ALLOCATOR_H

#include <CommProto/tools/allocator/Allocator.h>

namespace Comnet {
namespace Tools {
namespace Allocator {

template<typename _Ty>
class StackAllocator : public Allocator<_Ty>  {

public:
private:
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace
#endif // __STACK_ALLOCATOR_H
