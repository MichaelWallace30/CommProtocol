
#include <ABSPacket.h>
#include <CommProto/tools/allocator/commpointer.h>

namespace Comnet {

#pragma managed

AbstractPacketForwarder::AbstractPacketForwarder(ABSPacket^ packet)
: owner(packet)
, AbstractPacket(static_cast<const char*>(Marshal::StringToHGlobalAnsi(packet->GetPacketName()).ToPointer()))
{ 

}


void AbstractPacketForwarder::Pack(comnet::serialization::ObjectStream& obj) {
  CommPointer<comnet::serialization::ObjectStream> ptr = obj;
  Comnet::Serialization::ObjectStream^ wrapper = gcnew Comnet::Serialization::ObjectStream(&ptr);

  owner->Pack(wrapper);

  obj.SetBuffer((char*)ptr.Get().GetBuffer(), ptr.Get().GetSize());
}


void AbstractPacketForwarder::Unpack(comnet::serialization::ObjectStream& obj) {
  CommPointer<comnet::serialization::ObjectStream> ptr = obj;
  ObjectStream^ wrapper = gcnew Comnet::Serialization::ObjectStream(&ptr);

  owner->Unpack(wrapper);
  // ptr is now in control of the ptr;
}


AbstractPacket* AbstractPacketForwarder::Create() {
  ABSPacket^ packet = owner->Create();
  return packet->GetAbstractPacket();
}



}
