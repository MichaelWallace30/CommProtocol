/*
  Mutex object.

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
#ifndef __COMMMUTEX_H
#define __COMMMUTEX_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <algorithm>


namespace comnet {
namespace architecture {
namespace os {


// Mutex Wrapper for CommProtocol.
class CommMutex {
public:
  CommMutex() { }

  void Lock() { mu.lock(); }
  void Unlock() { mu.unlock(); }
  bool TryLock() { mu.try_lock(); }
  
  ::std::mutex& GetMutex() { return mu; }
private:
  ::std::mutex mu;
};


/**
  Comms lock is a standard lock for mutex, intended to allow locking for certain amounts of time
  and whatnot.
*/
class CommLock : ::std::unique_lock< ::std::mutex> {
public:
  CommLock(CommMutex& mut) : ::std::unique_lock< ::std::mutex>(mut.GetMutex()) { }
  ~CommLock() { }
private:
};



} // os
} // architecture
} // comnet
#endif // __COMMMUTEX_H