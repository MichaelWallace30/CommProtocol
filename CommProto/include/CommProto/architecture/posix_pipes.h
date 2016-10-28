/*
  Posix Pipe.

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
#ifndef __POSIX_PIPES_H
#define __POSIX_PIPES_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/pipes.h>


namespace comnet {
namespace architecture {


class COMM_EXPORT Pipe : public IPipe {
public:
private:
  pipe_t pipefd[2];
};
} // architecture
} // comnet
#endif // __POSIX_PIPES_H
