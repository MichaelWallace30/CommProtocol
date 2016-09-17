/*
  WPAN configurations for ZigBee 802.15.4 and Digimesh Protocols.

  Much thanks to Digi International Inc.

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

// ZigBee Stack Profile IDs.

// Network Specific
#define WPAN_STACK_PROFILE_PROPRIETARY           0x0
// ZigBee (2006)
#define WPAN_STACK_PROFILE_ZIGBEE                0x1
// ZigBee PRO (2007)
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
#define WPAN_ENDPOINT_ZIGBEE_DEVICE_OBJECT       0x00

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


// General handler used in cluster table.
typedef int (*wpan_aps_handler_fn)(const wpan_message_t* message, void* context);

// Declaration used at a later time.
struct wpan_ep_state_t;
// General handler for endpoint table.
typedef int (*wpan_ep_handler_fn)(const wpan_message_t* message, struct wpan_ep_state* ep_state);

//
// Cluster table entries.
// USed to associate clusters with an endpoint.
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

/**
  Endpoint cluster stuff, which contains information about each endpoint on this device.
*/
typedef struct wpan_endpoint_table_entry_t {

  uint8_t endpoint;
  uint16_t profile_id;
  wpan_ep_handler_fn handler;
  struct wpan_ep_state_t* ep_state;
  uint16_t device_id;
  uint8_t device_version;
  const wpan_clust_table_entry_t* clust_table;

} wpan_endpoint_table_entry_t;


#define WPAN_CLUST_END_OF_LIST                       0xFFFF

#define WPAN_CLLUST_ENTRY_LIST_END                   WPAN_CLUST_END_OF_LIST

/**
  Function called by the WPAN APS layer to send a frame out on the network.

  This is part of the glue that links the XBee layer with the WPAN/ZigBee
  layers.
*/
typedef int32_t (*wpan_endpoint_send_fn)(const wpan_message_t* msg, uint16_t flags);

#define WPAN_SEND_FLAG_NONE 0x0000

#define WPAN_SEND_FLAG_ENCRYPT 0x0001

/**
  Function called by the WPAN APS layer to configure the underlying network
  device.
*/
typedef int32_t (*wpan_config_fn)(struct wpan_dev_t* device, ...);
/**
  Function called by the WPAN APS layer to have the underlying device read
  and dispatch frames.
*/
typedef int32_t (*wpan_tick_fn)(struct wpan_dev_t* device);
/**
  Custom function for walking the endpoint table.
  The application can define its own function to walk the endpoint table,
  possibly to support a dynamic table or a table where endpoints aren't
  always visible.
*/
typedef const wpan_endpoint_table_entry_t* (*wpan_endpoint_get_next_fn)(
                    struct wpan_dev_t* device, const wpan_endpoint_table_entry_t* ep);

/**
  Device record that holds the ZigBee layers and information about this node.

  This happens to be an abstraction of the physical hardware and networking layer.
*/
typedef struct wpan_dev_t {

  wpan_tick_fn                  tick;
  wpan_endpoint_send_fn         endpoint_send;
  wpan_endpoint_get_next_fn     endpoint_next;
  wpan_address_t                addr;
  uint16_t                      payload;
  uint16_t                      flags;
  // Macros below are the flags used for flags.
#define WPAN_FLAG_NONE                            0x0000
#define WPAN_FLAG_JOINED                          0x0001
#define WPAN_FLAG_AUTHENTICATED                   0x0002
#define WPAN_FLAG_AUTHENTICATION_ENABLED          0x0004
  
  const wpan_endpoint_table_entry_t* endpoint_table;
} wpan_dev_t;

#define WPAN_DEV_IS_JOINED(dev)                ((dev)->flags & WPAN_FLAG_JOINED)
#define WPAN_DEV_IS_AUTHENTICATED(dev)         ((dev)->flags & WPAN_FLAG_AUTHENTICATED)

#define WPAN_ENDPOINT_END_OF_LIST              0xFF
#define WPAN_ENDPOINT_TABLE_END                WPAN_ENDPOINT_END_OF_LIST

int32_t wpan_tick(wpan_dev_t* dev);

const wpan_clust_table_entry_t* 
wpan_cluster_match(uint16_t match, uint8_t mask, const wpan_clust_table_entry_t* entry);

const wpan_endpoint_table_entry_t* 
wpan_endpoint_get_next(wpan_dev_t* dev, const wpan_endpoint_table_entry_t* ep);

const wpan_endpoint_table_entry_t* 
wpan_endpoint_match(wpan_dev_t* dev, uint8_t endpoint, uint16_t profile_id);

const wpan_endpoint_table_entry_t* 
wpan_endpoint_of_message(const wpan_message_t* msg);

const wpan_endpoint_table_entry_t* 
wpan_endpoint_of_cluster(wpan_dev_t* dev, uint16_t profile_id, uint16_t clust_id, uint8_t mask);

#define WPAN_APS_PROFILE_ANY 0xFFFF


// Message handling, for sending, receiving, replying and so forth.
void wpan_message_create(wpan_message_t* msg, wpan_dev_t* dev, const address64* ieee, uint16_t network_addr);
int32_t wpan_message_replay(wpan_message_t* replay, const wpan_message_t* original);

int32_t wpan_message_send(const wpan_message_t* msg);
void wpan_message_dump(const wpan_message_t* msg);


struct wpan_conversation_t;

typedef int32_t (*wpan_response_fn)(struct wpan_conversation_t* conversation, const wpan_message_t* msg);

#define WPAN_CONVERSATION_END                    1
#define WPAN_CONVERSTAION_CONTINUE               2


typedef struct wpan_conversation_t {

  uint8_t                     transaction_id;
  void*                       context;
  uint16_t                    timeout;
  wpan_response_fn            handler;

} wpan_conversation_t;

// Number of outstanding requests to track in a wpan_ep_state_t associated
// with an endpoint.
#ifndef WPAN_MAX_CONVERSATIONS
#define WPAN_MAX_CONVERSATIONS 3
#endif

// Default conversation recycle timeout (seconds)
#ifndef WPAN_CONVERSATION_TIMEOUT
#define WPAN_CONVERSATION_TIMEOUT 30
#endif


// Volatile part of an endpoint record used to track conversations
// (requests waiting for responses).
typedef struct wpan_ep_state_t {
  uint8_t					last_transaction;
  wpan_conversation_t	conversations[WPAN_MAX_CONVERSATIONS];
} wpan_ep_state_t;



int wpan_conversation_register(wpan_ep_state_t* state,
                               wpan_response_fn handler, const void* context, uint16_t timeout);

int wpan_conversation_response(wpan_ep_state_t* state,
                               uint8_t transaction_id, const wpan_message_t* envelope);
uint8_t wpan_endpoint_next_trans(const wpan_endpoint_table_entry_t *ep);

int wpan_envelope_dispatch(wpan_message_t* envelope);

void wpan_conversation_delete(wpan_conversation_t* conversation);
END_EXTERN_C

#endif // __WPAN_H