/*
  Runs a thread to update pingers.

  Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

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

#include <CommProto/ping/pingmanager.h>
#include <CommProto/comms.h>

namespace comnet {
namespace ping {
error_t PingCallback(const comnet::Header & header, PingPacket & packet, comnet::Comms & node)
{
  if (packet.isPing())
  {
    if (node.GetPingManager()->CanPong(header.source_id))
    {
      PingPacket pingPacket;
      pingPacket.setPing(false);
      node.Send(pingPacket, header.source_id);
    }
  }
  return CALLBACK_SUCCESS | CALLBACK_DESTROY_PACKET;
}

PingManager::PingManager(Comms* ownerComms)
  :std::enable_shared_from_this <PingManager>(), ownerComms(ownerComms)
{
    pingPacket = new PingPacket();
}

void PingManager::LinkPingCallback()
{
  ownerComms->LinkCallback(pingPacket, new Callback((comnet::callback_t)PingCallback));
}

void PingManager::SendPingPacket(uint8_t destID)
{
  PingPacket sendPacket;
  sendPacket.setPing(true);
  ownerComms->Send(sendPacket, destID);
}

PingManager::~PingManager()
{
  runningMutex.Lock();
  running = false;
  runningMutex.Unlock();
}

void PingManager::TransferToActivePingers(std::list<Pinger>::iterator it)
{
  activePingers.splice(activePingers.end(), inactivePingers, it);
  std::string debugMsg = "Pinger with destID ";
  debugMsg += std::to_string((int)it->GetDestID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)ownerComms->GetNodeId());
  debugMsg += " was set to active";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}

void PingManager::TransferToInactivePingers(std::list<Pinger>::iterator it)
{
  inactivePingers.splice(inactivePingers.end(), activePingers, it);
  std::string debugMsg = "Pinger with destID ";
  debugMsg += std::to_string((int)it->GetDestID());
  debugMsg += " in NodeID ";
  debugMsg += std::to_string((int)ownerComms->GetNodeId());
  debugMsg += " was set to inactive";
  comnet::debug::Log::Message(comnet::debug::LOG_DEBUG, debugMsg);
}

} //namespace ping
} //namespace comnet