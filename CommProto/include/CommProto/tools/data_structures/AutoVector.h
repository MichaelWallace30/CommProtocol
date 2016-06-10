/*
  Wrapper class for standard library vector.

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
#ifndef __AUTO_VECTOR_H
#define __AUTO_VECTOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
// Standard library vector.
#include <vector>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   AutoVector is a wrapper class for the standard library vector. The intentions 
   of this is so that the vector is not anymore complicated than it should be, since
   the standard library is not at all entirely supported on any platform, we must ensure
   that this wrapper class will garauntee that we are using the vector correctly, without
   having to look through lines of code figuring out why this vector function doesn't work.
*/
template<typename _Ty>
AutoVector : public Interface::List<_Ty> {
public:
  AutoVector() { 
    this->listType = Interface::AUTO_VECTOR;  
    this->size = 0;
  }
  
  ~AutoVector() { }

  

private:
  /**
     The vector we are wrapping.
  */
  std::vector<_Ty> _vector;
};
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __AUTO_VECTOR_H
