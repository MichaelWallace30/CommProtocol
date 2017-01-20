/*
*/
#ifndef __COMMPROTOCOL_OBSERVER_H
#define __COMMPROTOCOL_OBSERVER_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <functional>
#include <utility>


namespace comnet {
namespace tools {


/**
  Observer Object, used to observe an object.
*/
template<typename Obj,
         typename Return    = void,
         typename Function  = Return()>
class Observer {
  typedef std::function<Function> ObserverCallback;
public:

  Observer(Obj *obj = nullptr, ObserverCallback call = [] () -> Return { })
    : observed(obj)
    , callback(call)
  {
  }

  /**
    Obtain the observed object.
  */
  Obj *operator*() {
    return observed;
  }


  /**
    Observe something.
  */
  void Observe(Obj *obj) {
    observed = obj;
  }

  /**
    Update function, which uses the user defined function.
  */
  template<typename... Args>
  Return Update(Args ...args) {
    static_assert(std::is_function<Function>(), "Not a function!");
    if (!std::is_void<std::result_of<Function> >()) {
      return callback(std::forward<Args>(args)... );
    } 
    callback(std::forward<Args>(args)... );
  }

private:
  Obj *observed;
  ObserverCallback callback;
};
} // tools
} // comnet

#endif // __COMMPROTOCOL_OBSERVER_H