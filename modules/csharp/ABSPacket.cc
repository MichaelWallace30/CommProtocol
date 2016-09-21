#include <ABSPacket.h>
#include <CommProto/tools/allocator/CommPointer.h>

namespace Comnet {

#pragma managed

AbstractPacketForwarder::AbstractPacketForwarder(ABSPacket^ packet)
: owner(packet)
, AbstractPacket(static_cast<const char*>(Marshal::StringToHGlobalAnsi(packet->GetPacketName()).ToPointer()))
{ 

}


void AbstractPacketForwarder::pack(ObjectStream& obj) {
  CommPointer<ObjectStream> ptr = obj;
  CObjectStream^ wrapper = gcnew CObjectStream(&ptr);

  owner->Pack(wrapper);

  obj.setBuffer((char*)ptr.get().getBuffer(), ptr.get().getSize());
}


void AbstractPacketForwarder::unpack(ObjectStream& obj) {
  CommPointer<ObjectStream> ptr = obj;
  CObjectStream^ wrapper = gcnew CObjectStream(&ptr);

  owner->Unpack(wrapper);
  // ptr is now in control of the ptr;
}


AbstractPacket* AbstractPacketForwarder::create() {
  ABSPacket^ packet = owner->Create();
  return packet->GetAbstractPacket();
}



}
