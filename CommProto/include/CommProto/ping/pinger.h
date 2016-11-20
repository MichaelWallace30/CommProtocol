/*
		Represents a client that needs to be pinged to check if still connected.

		Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

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
#ifndef __PINGER_H
#define __PINGER_H

namespace comnet {
namespace ping {
class Pinger
{
public:
		Pinger();
		~Pinger();
};
} //namespace ping
} //namespace comnet
#endif //__PINGER_H
