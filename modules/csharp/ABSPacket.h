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
#ifndef __ABSPACKET_H
#define __ABSPACKET_H

#include <CommProto/AbstractPacket.h>
#include <CommProto/tools/allocator/CommPointer.h>
#include <ObjectStreamWrapper.h>
#include <vcclr.h>
#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Comnet {


using namespace Comnet::Tools::Allocator;
#pragma managed
ref class ABSPacket;

/**
  AbstractPacketForwarder intends on wrapping the AbstractPacket Abstract.
  This overrides the AbstractPacket Functions, and uses another abstract class,
  ABSPacket, which is managed.
*/
class AbstractPacketForwarder : public AbstractPacket {
public:
  AbstractPacketForwarder(ABSPacket^ packet);

  /**
    TODO(Garcia): This will cause problems because obj will be destroyed,
                  with ObjectStreamWrapper, after this function call.
          

    Trying out CommPointer for the first time, let this baby roll!
   */
  virtual void pack(ObjectStream& obj);

  virtual void unpack(ObjectStream& obj);

  virtual AbstractPacket* create();

private:
  gcroot<ABSPacket^> owner;
};

/**
  ABSPacket is a managed Abstract class, intended on managing AbstractPacketForwarder,  
  which is too responsible for the AbstractPacket Class.
*/
public ref class ABSPacket abstract {
public:

  virtual void Pack(ObjectStreamWrapper^ obj) = 0;
  virtual void Unpack(ObjectStreamWrapper^ obj) = 0;
  virtual ABSPacket^ Create() = 0;

  String^ GetPacketName() { return packetName; }

  AbstractPacket* getAbstractPacket() {
    return unmanaged;
  }

protected:
  ABSPacket(String^ name)
    : packetName(name) {
    unmanaged = new AbstractPacketForwarder(this);
  }
private:
  ABSPacket() { }
  AbstractPacket* unmanaged;
  String^ packetName;
};

}
#endif // __ABS_PACKET_H