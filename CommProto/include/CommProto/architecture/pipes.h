/*
  Base Pipe interface for Intercommunications.

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
#ifndef __PIPES_H
#define __PIPES_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <string>
#include <cstring>

#if COM_TARGET_OS == COM_OS_WINDOWS
 typedef PHANDLE pipe_t;
#else
 typedef int pipe_t;
#endif // COM_TARGET_OS == COM_OS_WINDOWS

namespace comnet {
namespace architecture {


enum PipeMode {
  READ,
  WRITE,
  BOTH,
};


_INTERFACE_ COMM_EXPORT IPipe {
  COMM_DISALLOW_COPYING(IPipe);
public:
  IPipe() = default;
  virtual ~IPipe() { }
  
  virtual bool Create() = 0;
  /**
    Named Pipe creation, win32 deals with these handles in a magical world called WINAPIs, 
    while linux deals with file paths, in FIFO format. both deal with reading/writing through a file.
  */
  virtual bool CreateNamed(std::string path, PipeMode mode) = 0;
  virtual bool Read(char* buf, uint32_t len) = 0;
  virtual bool Write(char* buf, uint32_t& len) = 0;
  virtual pipe_t& GetReadHandle() = 0;
  virtual pipe_t& GetWriteHandle() = 0;
};
} // architecture
} // comnet
#endif // __PIPES_H