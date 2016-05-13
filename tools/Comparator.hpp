#ifndef __COMPARATOR_H
#define __COMPARATOR_H

#include <architecture/os/include_defines.h>

namespace Comnet {

#define _LESS_THAN   -1
#define _EQUAL_TO     0
#define _GREATER_THAN 1 

template<typename _Ty>
class Comparator {
  typedef const _Ty& const_reference;
public:
  Comparator() { }
  virtual Comparator() { }

  /** Overloaded operator to compare values */
  virtual int32_t operator()(const_reference obj1, const_reference obj2) {
    return compare(obj1, obj2);
  }

  /** If comparator is needed, be sure to define any inherited classes with this 
      function. */
  virtual int32_t compare(const_reference obj1, const_reference obj2) {
    if (obj1 > obj2) {
      return _GREATER_THAN;
    } else {
      return _LESS_THAN;
    }
  }
private: 
};

} // Comnet 

#endif // __COMPARATOR_H
