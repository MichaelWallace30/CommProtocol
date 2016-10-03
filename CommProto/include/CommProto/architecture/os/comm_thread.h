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

#include <thread>
#include <utility>


namespace comnet {
namespace architecture {
namespace os {


/**
  Thread wrapper CommThread is intended to work as an interface.
 */
class CommThread : ::std::thread {
public:
  template<typename Function>
  CommThread(Function&& funct) 
  : ::std::thread::thread(std::forward<Function>(funct)) { }


  template<typename Function,
           typename Arg1>
  CommThread(Function&& funct, Arg1&& arg1)
  : ::std::thread::thread(std::forward<Function>(funct),
                          std::forward<Arg1>(arg1)) { }
private:

};
} // os
} // architecture
} // comnet
#endif // __COMMTHREAD_H