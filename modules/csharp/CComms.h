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
#ifndef __CCOMMS_H
#define __CCOMMS_H

#include <CCommNode.h>
#include <vcclr.h>
#using <mscorlib.dll>


namespace Comnet {

#pragma managed

// CComms node.
public ref class CComms : CCommNode {
public:

private:
  System::Threading::Thread sendThr;
  System::Threading::Thread recvThr;

  System::Threading::Mutex sendMut;
  System::Threading::Mutex recvMut;
};
}
#endif // __CCOMMS_H