/*
  These application programming interface tags will be used to tag functionality
  within our library. They serve as macros and definitions to clearly indicate what
  module or piece of code is part of the API and what should be hidden.

  Application Programming interface tags. 
  
  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __API_H
#define __API_H

#include <architecture/os/include_defines.h>

#define _COMNET_MINIMUM_API_VERSION  0
#define _COMNET_API_VERSION_         1

#define obtain_api_version(_api_version) _api_version

#define _COMNET_PUBLIC_API_
#define _COMNET_API_
#define _COMNET_PRIVATE_API_

#define _COMNET_API_LEVEL_UNKNOWN    
#define _COMNET_API_LEVEL_0_         0
#define _COMNET_API_LEVEL_1_         1
#define _COMNET_API_LEVEL_2_         2
#define _COMNET_API_LEVEL_3_         3
#define _COMNET_API_LEVEL_4_         4
#define _COMNET_API_LEVEL_5_         5


#define current_api_version obtain_api_version(_COMNET_API_VERSION_)

#endif // __API_H
