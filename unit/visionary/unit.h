//
//
//
#ifndef __VISIONARY_UNIT_H
#define __VISIONARY_UNIT_H

#include <functional>
#include <algorithm>
#include <utility>
#include <memory>

namespace visionary {



template<typename Result>
class BaseUnit {
  typedef std::function<Result> BaseFunct;
public:
 BaseUnit(BaseFunct f) : funct(f) { }

protected:
  BaseFunct funct;
};


template<typename Result, typename Function>
class BaseUnit<Result(Function)> {
  typedef std::function<Function> BaseFunct;
public:
  BaseUnit(BaseFunct f) : funct(f) { }

  Function GetFunct() { return funct; }
  Result Execute() { return funct(); }
protected:
  BaseFunct funct;
};
} // visionary
#endif // __VISIONARY_UNIT_H