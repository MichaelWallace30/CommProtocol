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
#ifndef __CSHARP_ABSPACKET_H
#define __CSHARP_ABSPACKET_H

#include <CommProto/abstractpacket.h>
#include <CommProto/tools/allocator/commpointer.h>
#include <ObjectStream.h>
#include <vcclr.h>
#using <mscorlib.dll>

using namespace System;

namespace Comnet {

using namespace Comnet::Serialization;
using namespace comnet::tools::allocator;
using namespace comnet;
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
  virtual void Pack(comnet::serialization::ObjectStream& obj);

  virtual void Unpack(comnet::serialization::ObjectStream& obj);

  virtual AbstractPacket* Create();

private:
  gcroot<ABSPacket^> owner;
};

/**
  ABSPacket is a managed Abstract class, intended on managing AbstractPacketForwarder,  
  which is too responsible for the AbstractPacket Class.
*/
public ref class ABSPacket abstract {
public:

  virtual void Pack(ObjectStream^ obj) = 0;
  virtual void Unpack(ObjectStream^ obj) = 0;
  virtual ABSPacket^ Create() = 0;

  String^ GetPacketName() { return packetName; }

  AbstractPacket* GetAbstractPacket() {
    return unmanaged;
  }

  void SetAbstractPacket(AbstractPacket* packet) {
    if (unmanaged) {
      delete unmanaged;
      unmanaged = nullptr;
    }

    unmanaged = packet;
  }

  generic<typename Type> where Type:ABSPacket
  static Type GetValue(ABSPacket^ packt) {
    try {
      return static_cast<Type>(packt);
    } catch (System::Exception^ e) {
      throw gcnew System::InvalidCastException(gcnew String("Invalid casting"));
    } 
  }

protected:
  ABSPacket(String^ name)
    : packetName(name)
  {
    unmanaged = new AbstractPacketForwarder(this);
  }
private:
  ABSPacket() { }
  AbstractPacket* unmanaged;
  String^ packetName;
};
}
#endif // __CSHARP_ABS_PACKET_H