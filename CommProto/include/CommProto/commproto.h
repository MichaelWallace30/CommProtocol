/*
  Standard CommProtocol Library header. Declarations of the library are included
  in this header file, provided by forward declaring, which is convenient for 
  users not having to figure out which files are going to be needed for their
  application. 

  Ensure that you link your static or shared CommProtocol library to your
  application prior to using this header file. 

  Take a look at our documentation for more information about the CommProtocol
  API functionality, and how to use this library for your application. All documentation
  can be generated with Doxygen. It is still a working progress, however, the library
  was created for ease of use, without too much worry about how everything works.

  

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia, Alex Craig.

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
#ifndef __COMMPROTO_H
#define __COMMPROTO_H

/*
  Common thread and mutex handling for developers. These are specially defined for 
  use in the library.
*/
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/comm_mutex.h>

/*
  ObjectStream library. This file defines the serial buffers used for 
  transmission and retrieval of data from end users. Not much needs to be
  described here, as the functionality is low level, meaning it is more
  complex in nature to describe.
*/
#include <CommProto/serialization/objectstream.h>

/*
  Standard Comparators that might be used by the library. Developers 
  can use the Comparator interfaces as they shall need to, in order to 
  define their own data structures for this library.
*/
#include <CommProto/tools/comparator.h>
#include <CommProto/tools/standard_comparator.h>

/*
  Debug suppression and unsuppression for developers to handle.
*/
#include <CommProto/debug/log.h>

/*
  Common Library headers that developers will likely be using for their 
  applications.
*/
#include <CommProto/abstractpacket.h>
#include <CommProto/commnode.h>
#include <CommProto/comms.h>
#include <CommProto/callback.h>
#include <CommProto/headerpacket.h>

/*
  Standard C++ library headers.
*/
#include <iostream>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <memory>

/*
  include files that you think will be used by the user, so that this
  file acts as a commons folder to be readily used as the API gateway.

  To be continued...
*/
#endif // __COMMPROTO_H