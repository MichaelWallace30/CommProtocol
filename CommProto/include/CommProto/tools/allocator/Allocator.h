/*
  Allocator interface.

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
#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/api.h>

namespace Comnet {
namespace Tools {
namespace Allocator {

template<typename _Ty>
class Allocator {
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef _Ty* pointer;
public:
  Allocator() { }
  virtual Allocator() { }

  virtual pointer allocate(uint32_t sizeN) = 0;

  virtual void deallocate(pointer p) = 0;

  virtual destruct(pointer p) = 0;

  virtual construct(pointer p, const_reference value) = 0;

  virtual pointer address(reference ref) = 0;

  virtual const_pointer address(const_reference ref) const = 0;
private:
};
} // Allocator namespace
} // Tools namespace
} // Comnet namespace

#endif // __ALLOCATOR_H
