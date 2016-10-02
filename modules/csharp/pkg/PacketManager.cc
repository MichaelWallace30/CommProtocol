#include <pkg/PacketManager.h>

namespace Comnet {
namespace Pkg {


Boolean PacketManager::Insert(ABSPacket^ packet, CallBack^ callback) {
  return table.Insert(packet, callback);
}


CallBack^ PacketManager::Get(ABSPacket^ key) {
  return table.GetCallback(key->GetAbstractPacket()->GetId());
}


Boolean PacketManager::Remove(ABSPacket^ key) {
  return table.Remove(key->GetAbstractPacket()->GetId());
}


Boolean PacketManager::Contains(ABSPacket^ key) {
  ABSPacket^ packet = table.GetPacket(key->GetAbstractPacket()->GetId());
  if (packet) {
    return true;
  }

  return false;
}


Boolean PacketManager::Contains(CallBack^ call) {
  throw gcnew System::NotImplementedException();
  return false; // for now...
}


Int32 PacketManager::GetSize() {
  return table.GetNumOfPairs();
}


Void PacketManager::Resize() {
  throw gcnew System::NotImplementedException();
}


ABSPacket^ PacketManager::ProduceFromId(UInt32 key) {
  ABSPacket^ packet = table.GetPacket(key);
  if (packet) {
    return factory.ProduceFromId(packet);
  }

  return nullptr;
}
}
}