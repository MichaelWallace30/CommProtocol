/*
  Standard Comparator for standard data types.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __STANDARD_COMPARATOR_H
#define __STANDARD_COMPARATOR_H

#include <CommProto/architecture/macros.h>
#include <CommProto/architecture/api.h>

#include <CommProto/tools/comparator.h>

namespace comnet {
namespace tools {

/**
   StandardComparator should be able to comparate value of standard data types.
   This includes, ints, unsigned ints, unsigned shorts, long longs, doubles, floats, 
   and chars. It is also specialized to handle char* strings.
*/
_COMNET_PUBLIC_API_
template<typename _Ty>
class COMM_EXPORT StandardComparator : public Comparator<_Ty> {
  typedef const _Ty& const_reference;
public:
  StandardComparator() { }

  ~StandardComparator() { }

  int32_t Compare(const_reference obj1, const_reference obj2) {
    if (obj1 == obj2) {
      return 0;
    } else if (obj1 < obj2) {
      return 1;
    } else {
      return -1;
    }
  }

  bool Equal(const_reference obj1, const_reference obj2) {
    if (obj1 == obj2) {
      return true;
    } else {
      return false;
    }
  }
private:
};

/**
   Full template specialized version of StandardComparator to handle char* strings.
 */
template<> 
class StandardComparator<char*> : public Comparator<char*> {
  typedef const char* const_CString;
public:
  StandardComparator() { }

  ~StandardComparator() { }
  
  int32_t Compare(const_CString string1, const_CString string2) {
    return 0;
  }

  bool Equal(const_CString string1, const_CString string2) {
    return false;
  }
private:
};
} // Tools namespace 
} // Comnet namespace


#endif // __STANDARD_COMPARATOR_H
