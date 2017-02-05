/*
Condition variable.

Copyright (C) 2016  Alex Craig.

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
#ifndef __COMM_CONDVAR_H
#define __COMM_CONDVAR_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/cond.h>

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <algorithm>


namespace comnet {
  namespace architecture {
    namespace os {


      // ConditionVariable Wrapper for CommProtocol.
      class COMM_EXPORT CommConditionVariable {
      public:
        CommConditionVariable(bool ready = false)
          :ready(ready)
        {
        }

        /**
        Block the thread until Set
        */
        FORCE_INLINE void Wait() {
          std::unique_lock<std::mutex> lk(mu);
          cv.wait(lk, [&] {return ready; });
          ready = false;
        }
        /**
        Block the thread until Set or specified milliseconds pass
        */
        FORCE_INLINE void Wait(std::chrono::milliseconds waitMillis) {
          std::unique_lock<std::mutex> lk(mu);
          cv.wait_for(lk, waitMillis, [&]{return ready;});
          ready = false;
        }

        /**
        Ends the Wait
        */
        FORCE_INLINE void Set() { 
          std::lock_guard<std::mutex> lk(mu);
          ready = true;
          cv.notify_one();
        }
      private:
        std::condition_variable cv;
        std::mutex mu;
        bool ready;
      };
    } // os
  } // architecture
} // comnet
#endif // __COMM_COND_VAR_H
