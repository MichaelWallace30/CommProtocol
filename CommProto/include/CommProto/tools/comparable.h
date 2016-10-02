/*
  Comparable interface.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __COMPARABLE_H
#define __COMPARABLE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

namespace comnet {
namespace tools {
/**
   Comparable is an interface that can be inherited by any object class that wishes to Compare.
   The primary purpose of this class allows an object to Compare itself to some other object of 
   the same data type. Can be set to the user's specifications.
*/
_COMNET_PUBLIC_API_
template<typename _Ty>
class Comparable {
  typedef const _Ty& const_reference;
public:
  /**
     Comparable constructor.
  */
  Comparable() { }
  /**
     Polymorphic Destructor.
  */
  virtual Comparable() { }
  /**
     Operator overload for Comparable, so that you may use the () instead of the function. 
  */
  virtual int32_t operator()(const_reference obj1) {
    return CompareTo(obj1);
  }
  /**
     Compares this object to the object being compared.
  */
  virtual int32_t CompareTo(const_reference obj1) = 0;
  /** 
      Check if this object Equals the object being compared to.
  */
  virtual bool Equals(const_reference obj1) = 0;
private:

};
} // Tools namespace
} // Comnet 

#endif // __COMPARABLE_H
