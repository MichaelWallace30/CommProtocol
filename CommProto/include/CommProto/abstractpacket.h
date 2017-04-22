/*
  Abstract Packet implementation. Used to connect packets to the Communication
  node.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

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
#ifndef __ABSTRACT_PACKET_H
#define __ABSTRACT_PACKET_H

#include <CommProto/headerpacket.h>
#include <CommProto/serialization/objectstream.h>

#include <exception>
#include <typeinfo>

namespace comnet {


// This class needs to be created by us. An Object stream will need to be serializing and deserializing.
using namespace serialization;

/** 
    Use this AbstractPacket when it comes to packing, sending , receiving, and creating.

    To define an abstract packet functions Pack, Unpack, and Create must be defined. 
    ALSO!!!!! Before your constructor, you must define name of the packet you are creating (not required
    but HIGHLY recommended).
*/
class COMM_EXPORT AbstractPacket {

  COMM_DISALLOW_COPYING(AbstractPacket);

public:
  /**
     Allows us to define the type of class we are working with.
  */
  AbstractPacket(const char* className);
  /**
     Make this abstract class polymorphic.
  */
  virtual ~AbstractPacket();
  /**
     Get the id of the AbstractPacket. This will specialize what type of 
     packet it is.
   */
  uint32_t GetId() const
    { return type_id; }

  /**
     Packing should be defined by the user.
  */
  virtual void Pack(ObjectStream& obj) = 0;
  /**
     Unpacking should be defined by the user.
  */
  virtual void Unpack(ObjectStream& obj) = 0;
  /**
     Creates a packet.
   */
  virtual AbstractPacket* Create() = 0;

  template<typename Type>
  static Type& GetValue(AbstractPacket& packet) NOEXCEPT {
    try {
      return dynamic_cast< Type& >( packet );
    } catch ( std::bad_cast e ) {
      // TODO(Garcia): Will require Using a Logger instead of COMMS_DEBUG.
      LOG_PRINTF(debug::LOG_ERROR, "Bad casting for packet, which is of type %s\n", typeid(packet).name());
    }
  }

private:
  /**
     Default constructor, yet this is a bit of a dangerous thing to use.
   */
  AbstractPacket() 
    { }
  /**
     This id defines the type of object.
  */
  uint32_t type_id;

};

// Small defines for inheriting C++ AbstractPacket
#define CHAIN_ABSPACKET(class_name) comnet::AbstractPacket(#class_name)
#define INHERITS_ABSPACKET public comnet::AbstractPacket
#define ABSPACKET comnet::AbstractPacket
typedef comnet::AbstractPacket ABSPacket;


} // Comnet namespace 
#endif // __ABSTRACT_PACKET_H
