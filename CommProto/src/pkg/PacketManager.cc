/*
Packet Manager.

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
#include <CommProto/pkg/packetmanager.h>
#include <CommProto/pkg/packettable.h>
#include <CommProto/abstractpacket.h>
#include <CommProto/callback.h>

#define DEFAULT_TABLE_SIZE 1000

namespace comnet {
namespace pkg {


PacketManager::PacketManager()
  : table(new PacketTable(DEFAULT_TABLE_SIZE))
{
}


PacketManager::PacketManager(uint32_t setSize)
  : table(new PacketTable(setSize))
{
}


PacketManager::~PacketManager() 
{
  delete table;
  nullify_pointer(table);
}


bool PacketManager::insert(const AbstractPacket* key, const Callback* callback) {
  return table->insert(key, callback);
}


Callback* PacketManager::get(const AbstractPacket& key) {
  return table->getCallback(key.getId());
}


AbstractPacket* PacketManager::produceFromId(uint32_t key) {
  AbstractPacket* packet = table->getPacket(key);
  return factory.produceNewPacket(packet);
}


bool PacketManager::remove(const AbstractPacket& key) {
  return table->remove(key.getId());
}
} // namespace Pkg
} // namespace Comnet
