/*
Packet Manager.

Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

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
#include <CommProto/pkg/packetmanager.h>
#include <CommProto/pkg/packethashtable.h>
#include <CommProto/abstractpacket.h>
#include <CommProto/callback.h>

namespace comnet {
namespace pkg {


PacketManager::PacketManager()
  : table(new PacketHashTable())
{
}


PacketManager::PacketManager(uint32_t setSize)
  : table(new PacketHashTable(setSize))
{
}


PacketManager::~PacketManager() 
{
  free_pointer(table);
}


bool PacketManager::Insert(const AbstractPacket* key, const Callback* callback) {
  return table->Insert(key, callback);
}


Callback* PacketManager::Get(const AbstractPacket& key) {
  return table->GetCallback(key.GetId());
}

uint32_t PacketManager::GetSize()
{
  return table->getNumOfPairs();
}

bool PacketManager::Reserve(uint32_t size)
{
  return table->Reserve(size);
}

AbstractPacket* PacketManager::ProduceFromId(uint32_t key) {
  AbstractPacket* packet = table->GetPacket(key);
  return factory.ProduceNewPacket(packet);
} 


bool PacketManager::Remove(const AbstractPacket& key) {
  return table->Remove(key.GetId());
}
bool PacketManager::Contains(const AbstractPacket & key)
{
  return (table->GetPacket(key.GetId()) != NULL);
}
bool PacketManager::Contains(Callback * call)
{
  return (table->Contains(call));
}
} // namespace Pkg
} // namespace Comnet
