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
#ifndef __CPACKET_MANAGER_H
#define __CPACKET_MANAGER_H

#include <CommProto/pkg/packetmanager.h>

#include <pkg/PacketFactory.h>
#include <pkg/PacketTable.h>
#include <CallBack.h>
#include <ABSPacket.h>


#include <vcclr.h>
#using <mscorlib.dll>


using namespace System;

namespace Comnet {
namespace Pkg {

#pragma managed

/**
  CPacketManager is an interface, meant to allow interchangeable hashing and managing.
*/
public ref class PacketManager {
public:

  Boolean Insert(ABSPacket^ key, CallBack^ callback);
  CallBack^ Get(ABSPacket^ key);
  Boolean Remove(ABSPacket^ key);
  Boolean Contains(ABSPacket^ key);
  Boolean Contains(CallBack^ call);
  Int32 GetSize();
  Void Resize();

  ABSPacket^ ProduceFromId(UInt32 key);
private:
  PacketFactory factory;
  PacketTable table;
};
}
}
#endif // __CPACKET_MANAGER_H