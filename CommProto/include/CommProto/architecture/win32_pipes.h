/*
  Win32 pipe.

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
#ifndef __WIN32_PIPES_H
#define __WIN32_PIPES_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/pipes.h>

namespace comnet {
namespace architecture {


class COMM_EXPORT Pipe : public IPipe {
  COMM_DISALLOW_COPYING(Pipe);
public:
  Pipe() { }

  /**
    Creates an anonymous local pipe, used for local communications to CommProtocol.
    @param mode The mode of which you want this pipe to be using. This allows reading
                one way, or writing one way, or none, or both.
  */
  bool Create(PipeMode mode) {
    bool success = false;
    // Create an anonymous local pipe for windows.
    switch (mode) {
      case READ:
        break;
      case WRITE:
        break;
      default:
    };
    success = CreatePipe(rpipe, wpipe, NULL, 0);
    return success; 
  }


  bool CreateNamed(std::string path, PipeMode mode);
  bool Connect(std::string path, PipeMode mode);
  bool Read(char* buf, uint32_t& len);
  bool Write(char* buf, uint32_t len);
  bool Close() {
    CloseHandle(rpipe);
    CloseHandle(wpipe);
  }
  pipe_t& GetReadHandle() { return rpipe; }
  pipe_t& GetWriteHandle() { return wpipe; }
  PipeStatus GetStatus() { return status; }
  PipeType GetType() { return type; }
private:
  pipe_t rpipe;
  pipe_t wpipe;
  PipeType type;
  PipeStatus status;
};
} // architecture
} // comnet
#endif // __WIN32_PIPES_H