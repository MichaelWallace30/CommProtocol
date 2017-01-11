#include <ping/PingPacket.h>

namespace Comnet {
  namespace Ping {
    PingPacket::PingPacket()
      :ABSPacket("PingPacket"), ping(false)
    {

    }

    Void PingPacket::Pack(ObjectStream^ objOut)
    {
      Byte pingByte = (Byte)ping;
      objOut->Input(pingByte);
    }

    Void PingPacket::Unpack(ObjectStream^ objIn)
    {
      Byte pingByte = objIn->OutputByte();
      ping = (bool)pingByte;
    }
  }
}