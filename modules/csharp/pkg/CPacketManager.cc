#include <pkg/CPacketManager.h>

namespace Comnet {
namespace Pkg {


Boolean CPacketManager::Insert(ABSPacket^ packet, CallBack^ callback) {
  return table.Insert(packet, callback);
}


CallBack^ CPacketManager::Get(ABSPacket^ key) {
  return table.GetCallback(key->GetAbstractPacket()->getId());
}


Boolean CPacketManager::Remove(ABSPacket^ key) {
  return table.Remove(key->GetAbstractPacket()->getId());
}


Boolean CPacketManager::Contains(ABSPacket^ key) {
  ABSPacket^ packet = table.GetPacket(key->GetAbstractPacket()->getId());
  if (packet) {
    return true;
  }

  return false;
}


Boolean CPacketManager::Contains(CallBack^ call) {
  throw gcnew System::NotImplementedException();
  return false; // for now...
}


Int32 CPacketManager::GetSize() {
  return table.GetNumOfPairs();
}


Void CPacketManager::Resize() {
  throw gcnew System::NotImplementedException();
}


ABSPacket^ CPacketManager::ProduceFromId(UInt32 key) {
  ABSPacket^ packet = table.GetPacket(key);
  if (packet) {
    return factory.ProduceFromId(packet);
  }

  return nullptr;
}
}
}