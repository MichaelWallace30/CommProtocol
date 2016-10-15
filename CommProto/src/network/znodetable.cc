/*
  Zigbee Node Table.

  Copyright (C) 2016  Mario Garcia.

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
#include <CommProto/network/znodetable.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <xbee/platform.h>
#include <xbee/atcmd.h>


namespace comnet {
namespace network {


xbee_node_id_t node_table[NODE_TABLE_SIZE] = { { { { 0 } } } };


xbee_node_id_t* NodeByAddr(const addr64* ieee_addr) {
  xbee_node_id_t* rec;
  for (rec = node_table; rec < &node_table[NODE_TABLE_SIZE]; ++rec) {
    if (addr64_equal(ieee_addr, &rec->ieee_addr_be)) {
      return rec;
    }
  }
  return NULL;
}


xbee_node_id_t* NodeByName(const char* name) {
  xbee_node_id_t* rec;
  for (rec = node_table; rec < &node_table[NODE_TABLE_SIZE]; ++rec) {
    if (!addr64_is_zero(&rec->ieee_addr_be) 
        && strcmp(rec->node_info, name) == 0) {
      return rec;
    }
  }
  return NULL;
}


xbee_node_id_t* NodeByIndex(int idx) {
  xbee_node_id_t* rec;
  for(rec = node_table; rec < &node_table[NODE_TABLE_SIZE]; ++rec) {
    rec = &node_table[idx];
    if (!addr64_is_zero(&rec->ieee_addr_be)) {
      return rec;
    }
  }
  return NULL;
}


xbee_node_id_t* NodeAdd(const xbee_node_id_t* node_id) {
  xbee_node_id_t *rec, *copy = NULL;
  for (rec = node_table; rec < &node_table[NODE_TABLE_SIZE]; ++rec) {
    if (addr64_equal(&node_id->ieee_addr_be, &rec->ieee_addr_be)) {
      copy = rec;
      break;
    }
    if (copy == NULL && addr64_is_zero(&rec->ieee_addr_be)) {
      copy = rec;
    }
  }
  if (copy != NULL) {
    *copy = *node_id;
  }
  return copy;
}


void NodeTableDump() {
  xbee_node_id_t *rec;
  for(rec = node_table; rec < &node_table[NODE_TABLE_SIZE]; ++rec) {
    if(!addr64_is_zero(&rec->ieee_addr_be)) {
      printf("%2u: ", (int)(rec - node_table));
      xbee_disc_node_id_dump(rec);
    }
  }
}
} // network
} // comnet