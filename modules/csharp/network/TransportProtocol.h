/*
Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef __TRANSPORT_PROTOCOL_H
#define __TRANSPORT_PROTOCOL_H


#include <vcclr.h>
#using <mscorlib.dll>


namespace Comnet {
namespace Network {
#pragma managed


public enum class TransportProtocol {
  UDP_LINK,
  SERIAL_LINK,
  ZIGBEE_LINK,
  TCP_LINK
};
}
}
#endif // __TRANSPORT_PROTOCOL_H