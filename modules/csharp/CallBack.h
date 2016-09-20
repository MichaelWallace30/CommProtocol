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
#ifndef __CSHARP_CALLBACK_H
#define __CSHARP_CALLBACK_H

#include <CommProto/Callback.h>

#include <vcclr.h>
#using <mscorlib.dll>


using namespace System;
using namespace System::Runtime::InteropServices;

namespace Comnet {


public ref class CallBack {
public:

private:
  Callback* callback;
};
}
#endif // __CSHARP_CALLACK_H