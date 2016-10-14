/*
Zigbee node table configurations.

Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#ifndef __ZNODETABLE_H
#define __ZNODETABLE_H

#include <CommProto/architecture/api.h>
#include <xbee/discovery.h>


namespace comnet {
namespace network {


#define NODE_TABLE_SIZE  10
extern xbee_node_id_t node_table[NODE_TABLE_SIZE];


COMM_EXPORT xbee_node_id_t* NodeByAddr(const addr64* ieee_addr);
COMM_EXPORT xbee_node_id_t* NodeByName(const char* name);
COMM_EXPORT xbee_node_id_t* NodeByIndex(int idx);
COMM_EXPORT xbee_node_id_t* NodeAdd(const xbee_node_id_t* node_id);
COMM_EXPORT void NodeTableDump();
} // network 
} // comnet
#endif // __ZNODETABLE_H