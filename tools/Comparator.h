#ifndef __COMPARATOR_H
#define __COMPARATOR_H

#include <architecture/os/include_defines.h>

namespace Comnet {

#define _LESS_THAN   -1
#define _EQUAL_TO     0
#define _GREATER_THAN 1 

/**
   Comparator is an abstract class that compares to similar objects together.
   This class must be inherited by class intended on comparing to objects with
   similar data types.
*/
template<typename _Ty>
class Comparator {
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
      Overloaded operator to compare values 
  */
  virtual int32_t operator()(const_reference obj1, const_reference obj2) {
    return compare(obj1, obj2);
  }

  /** 
      If comparator is needed, be sure to define any inherited classes with this 
      function. 
  */
  virtual int32_t compare(const_reference  obj1, const_reference obj2) {
    return -100;
  }
  /**
     Check if the objects are equal.
  */
  virtual bool equal(const_reference obj1, const_reference obj2) {
    return -100;
  }

private: 
};

} // Comnet 

#endif // __COMPARATOR_H
