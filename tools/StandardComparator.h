#ifndef __STANDARD_COMPARATOR_H
#define __STANDARD_COMPARATOR_H

#include <architecture/macros.h>
#include <tools/Comparator.h>

namespace Comnet {
namespace Tools {

/**
   StandardComparator should be able to comparate value of standard data types.
   This includes, ints, unsigned ints, unsigned shorts, long longs, doubles, floats, 
   and chars. It is also specialized to handle char* strings.
*/
template<typename _Ty>
class StandardComparator : public Comparator<_Ty> {
  typedef const _Ty& const_reference;
public:
  StandardComparator() { }

  ~StandardComparator() { }

  int32_t compare(const_reference obj1, const_reference obj2) {
    if (obj1 == obj2) {
      return 0;
    } else if (obj1 < obj2) {
      return 1;
    } else {
      return -1;
    }
  }

  bool equal(const_reference obj1, const_reference obj2) {
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
  
  int32_t compare(const_CString string1, const_CString string2) {
    return 0;
  }

  bool equal(const_CString string1, const_CString string2) {
    return false;
  }
private:
};
} // Tools namespace 
} // Comnet namespace


#endif // __STANDARD_COMPARATOR_H
