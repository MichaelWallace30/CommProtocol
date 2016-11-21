#include <CommProto/ping/pingpacket.h>
#include <iostream>

namespace comnet {
namespace ping {

PingPacket::PingPacket()
  :CHAIN_ABSPACKET(PingPacket)
{
}

void PingPacket::Pack(REF_OBJECTSTREAM objOut)
{
		std::string str = (ping ? "I" : "O");
		objOut << str;
}

void PingPacket::Unpack(REF_OBJECTSTREAM objIn)
{
		std::string str;
		objIn >> str;
		if (str == "I")
		{
				ping = true;
		}
		else
		{
				ping = false;
		}
}

PingPacket::~PingPacket()
{
}

} //namespace ping
} //namespace comnet
