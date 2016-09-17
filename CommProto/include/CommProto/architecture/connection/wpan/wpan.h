/*
  WPAN configurations for ZigBee 802.14.1 and Digimesh Protocols.

Application Programming interface tags.

Copyright (C) 2016  Mario Garcia.

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
#ifndef __WPAN_H
#define __WPAN_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/wpan/wpan_address.h>


COM_EXTERN_C


#define WPAN_STACK_PROFILE_PROPRIETARY           0x0

#define WPAN_STACK_PROFILE_ZIGBEE                0x1

#define WPAN_STACK_PROFILE_ZIGBEE_PRO            0x2



// 
//  WPAN profiles.
//
#define WPAN_PROFILE_ZDO                         0x0000

#define WPAN_PROFILE_SMART_ENERGY                0x0109

#define WPAN_PROFILE_DIGIMESH                    0xC105


//
// WPAN fixed endpoints.
//
#define WPAN_ENDPOINT_ZIGBEE                     0x00

#define WPAN_ENDPOINT_DIGIMESH_SE                0x5E

#define WPAN_ENDPOINT_DIGIMESH_DEVICE_OBJECTS    0xE6

#define WPAN_ENDPOINT_DIGIMESH_DATA              0xE8

#define WPAN_ENDPOINT_BROADCAST                  0xFF


#define WPAN_ENDPOINT

// Digimesh cluster IDs for WPAN_ENDPOINT_DIGIMESH_DATA
enum wpan_digi_cluster {
  DIGI_CLUSTER_SLEEP_SYNC                 = 0x0009,
  DIGI_CLUSTER_SERIAL                     = 0x0011,
  DIGI_CLUSTER_LOOPBACK                   = 0x0012,
  DIGI_CLUSTER_ND_COMMAND                 = 0x0015,
  DIGI_CLUSTER_DN_COMMND                  = 0x0016,
  DIGI_CLUSTER_REMOTE_COMMAND             = 0x0021,
  DIGI_CLUSTER_NR_COMMAND                 = 0x0022,
  DIGI_CLUSTER_IODATA                     = 0x0092,
  DIGI_CLUSTER_WATCHPORT                  = 0x0094,
  DIGI_CLUSTER_NODEID_MESSAGE             = 0x0065,
  DIGI_CLUSTER_DN_RESPONSE                = 0x0096,
  DIGI_CLUSTER_REMOTE_RESPONSE            = 0x00A1,
  DIGI_CLUSTER_NR_RESPONSE                = 0x00A2,
  DIGI_CLUSTER_INDIRECT_ROUTE_ERR         = 0x00B0,
  DIGI_CLUSTER_PROG_XBEE_OTA_UPD          = 0x1000,
  DIGI_CLUSTER_NBRWUPDATE                 = 0x71FE,
  DIGI_CLUSTER_REMFWUPDATE                = 0x71FF,
  DIGI_CLUSTER_FWUPDATERESP               = (int32_t )0xF1FF,  
};

// Message used in wpan communications, used for Zigbee and Digimesh communications.
typedef struct wpan_message_t {

  struct wpan_dev_t*        dev;
  address64                 ieee_addr;
  uint16_t                  network_addr;
  uint16_t                  profile_id;
  uint16_t                  cluster_id;
  uint8_t                   src_endpoint;
  uint8_t                   dest_endpoint;
  uint16_t                  options;

  const void*               payload;
  uint16_t                  payload_length;

} wpan_message_t;



typedef int (*wpan_aps_handler_fn)(const wpan_message_t* message, void* context);

struct wpan_ep_state_t;
typedef int (*wpan_ep_handler_fn)(const wpan_message_t* message, struct wpan_ep_state* ep_state);

//
// Cluster table entries.
//

typedef struct wpan_clust_table_entry_t {

  uint16_t                cluster_id;
  wpan_aps_handler_fn     handler;
  const void*             context;
  uint8_t                 flags;
// Use these flags for cluster table.
#define WPAN_CLUSTER_FLAG_NONE                        0x00
#define WPAN_CLUSTER_FLAG_INPUT                       0x01
#define WPAN_CLUSTER_FLAG_OUTPUT                      0x02
#define WPAN_CLUSTER_FLAG_INOUT                       0x03
#define WPAN_CLUSTER_FLAG_SERVER                      WPAN_CLUSTER_FLAG_INPUT
#define WPAN_CLUSTER_FLAG_CLIENT                      WPAN_CLUSTER_FLAG_OUTPUT
#define WPAN_CLUSTER_FLAG_ENCRYPT                     0x10
#define WPAN_CLUSTER_FLAG_ENCRYPT_UNICAST             0x20
#define WPAN_CLUSTER_FLAG_NOT_ZIGBEE_CLUSTER_L        0x80

} wpan_clust_table_entry_t;

// Still working on this!!

typedef struct wpan_endpoint_table_entry_t {



} wpan_endpoint_table_entry_t;





END_EXTERN_C

#endif // __WPAN_H