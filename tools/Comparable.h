#ifndef __COMPARABLE_H
#define __COMPARABLE_H

#include <architecture/os/include_defines.h>

namespace Comnet {

/**
   Comparable is an interface that can be inherited by any object class that wishes to compare.
   The primary purpose of this class allows an object to compare itself to some other object of 
   the same data type. Can be set to the user's specifications.
*/
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
    return compareTo(obj1);
  }
  /**
     Compares this object to the object being compared.
  */
  virtual int32_t compareTo(const_reference obj1) = 0;
  /** 
      Check if this object equals the object being compared to.
  */
  virtual bool equals(const_reference obj1) = 0;
private:

};

} // Comnet 

#endif // __COMPARABLE_H
