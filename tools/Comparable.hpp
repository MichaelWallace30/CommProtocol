#ifndef __COMPARABLE_H
#define __COMPARABLE_H

#include <architecture/os/include_defines.h>

namespace Comnet {

template<typename _Ty>
class Comparable {
  typedef const _Ty& const_reference;
public:
  Comparable() { }
  virtual Comparable() { }

  virtual int32_t operator()(const_reference obj1) {
    return compareTo(obj1);
  }

  virtual int32_t compareTo(const_reference obj1) = 0;
  virtual bool equals(const_reference obj1) = 0;
private:

};

} // Comnet 

#endif // __COMPARABLE_H
