/*
  Piping for Intercommunications.

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
#ifndef __PIPING_H
#define __PIPING_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>


namespace coment {
namespace console {


/**
  Pipe is an object abstraction, used to handle intercommunication processes between the 
  CommNode and external applications. In order to communicate asyncronously with CommNode,
  We use a separate pipe to an external application, for use with both applications, since two
  CommNodes will need to be used. 
*/
class CommPipe {
public:
  
private:
};
} // console
} // coment
#endif // __PIPING_H