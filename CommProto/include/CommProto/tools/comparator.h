/*
  Comparator interface.

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
#ifndef __COMPARATOR_H
#define __COMPARATOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#define COMPARATOR_NOT_DEFINED  -999

#define _LESS_THAN                -1
#define _EQUAL_TO                  0
#define _GREATER_THAN              1

namespace comnet { 
namespace tools {
/**
   Comparator is an abstract class that compares to similar objects together.
   This class must be inherited by class intended on comparing to objects with
   similar data types.
*/
_COMNET_PUBLIC_API_
template<typename _Ty>
class COMM_EXPORT Comparator {
  typedef const _Ty& const_reference;
public:
  /**
     Comparator Constructor.
  */
  Comparator() { }
  /**
     Polymorphic Destructor.
  */
  virtual ~Comparator() { }

  /** 
      Overloaded operator to Compare values 
  */
  virtual int32_t operator()(const_reference obj1, const_reference obj2) {
    return Compare(obj1, obj2);
  }

  /** 
      If comparator is needed, be sure to define any inherited classes with this 
      function. 
  */
  virtual int32_t Compare(const_reference  obj1, const_reference obj2) = 0;
  /**
     Check if the objects are Equal.
  */
  virtual bool Equal(const_reference obj1, const_reference obj2) = 0;
private: 
};
} // Tools
} // Comnet 

#endif // __COMPARATOR_H
