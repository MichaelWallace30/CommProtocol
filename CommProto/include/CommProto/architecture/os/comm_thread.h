/*
  C++11 standard thread wrapper.

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
#ifndef __COMMTHREAD_H
#define __COMMTHREAD_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <thread>
#include <functional>
#include <utility>

namespace comnet {
namespace architecture {
namespace os {


/**
  CommThread is intended to work as a wrapper for implementation defined threads, specifically
  for operating system stuff. In order to remain cross compatible, we implement C++11 threads 
  rather than standalone C threads.
 */
class COMM_EXPORT CommThread {

  COMM_DISALLOW_COPYING(CommThread);

public:
  ~CommThread() { }


  /**
    Thread default constructor.
   */
  CommThread() { }


  /**
    Thread Function.
  */
  template<typename Function,
           typename... Args>
  CommThread(Function&& funct, Args&&... args) 
  : thr(::std::thread(std::forward<Function>(funct), 
                      std::forward<Args>(args)...)) { }


  /**
    Moves the thread functionality to this thread.
  */
  CommThread(CommThread&& com)
  : thr(std::move(com.thr)) { }


  CommThread& operator=(CommThread&& com) {
    thr = std::move(com.thr);
    return *this;
  }
  /**
    Safely detaches this thread from the current thread.
   */
  void Detach() {
    thr.detach();
  }


  /**
    This function should cause main thread to wait until this thread 
    is finished.
  */
  void Join() { thr.join(); }


  /**
    Determines whether this thread is joinable, or able to join in parallel with main thread.
  */
  bool IsJoinable() { return thr.joinable(); }


  /**
    Grabs the id of this thread.
   */
  ::std::thread::id GetId() { return thr.get_id(); }
private:
  ::std::thread thr;
};
} // os
} // architecture
} // comnet
#endif // __COMMTHREAD_H