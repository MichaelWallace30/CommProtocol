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

#include <CommProto/architecture/os/include_defines.h>


#define _COMNET_MINIMUM_API_VERSION_            0x1000
#define _COMNET_COMPATIBLE_API_VERSION_         0x1001
#define _COMNET_CURRENT_API_VERSION_            0x1001

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

#define _CLASS_                      class
#define _COMNET_INTERFACE_           
#define _COMNET_ABSTRACT_

#define current_api_version obtain_api_version(_COMNET_CURRENT_API_VERSION_)

//////////////////////////////////////////////////////////////////////////////////////
// Post testing for the latest API Version.
//
//////////////////////////////////////////////////////////////////////////////////////
#if (_COMNET_CURRENT_API_VERSION_ < _COMNET_MINIMUM_API_VERSION_)
 #error "This CommProtocol version will no longer work with other modules. Please update CommProtocol to the latest version."
#elif (_COMNET_CURRENT_API_VERSION_ < _COMNET_COMPATIBLE_API_VERSION_)
 #if (COM_TARGET_OS == COM_OS_WINDOWS)
  #pragma message ("This CommProtocol version is not recommended for use, as some features may be deprecated, or no longer supported.")
  #pragma message ("Consider updating your CommProtocol Library to the latest version.")
 #else
  #warning "This CommProtocol version is not recommended for use, as some features may be deprecated, or no longer supported."
  #warning "Consider updating your CommProtocol Library to the latest version."
 #endif // COM_TARGET_OS == COM_OS_WINDOWS
#endif
#endif // __API_H
