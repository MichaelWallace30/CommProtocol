/*
XBee Link that handles all xbee connections.

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
#include <CommProto/network/xbee.h>
#include <CommProto/network/znodetable.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/headerpacket.h>

#include <regex>
#include <string>
#include <ctype.h>

#include <xbee/atcmd.h>
#include <xbee/byteorder.h>
#include <xbee/device.h>
#include <xbee/discovery.h>
#include <xbee/transparent_serial.h>
#include <xbee/wpan.h>

#include <wpan/aps.h>
#include <wpan/types.h>
#include <zigbee/zdo.h>

/**
  TODO(Garcia): UNSAFE! this message handler will accept any messages!
*/
void TransparentDump(const addr64* ieee, const void* payload,
                      uint16_t length) {
  using namespace comnet::network::experimental;
  xbee_node_id_t *node_id;
  char buffer[ADDR64_STRING_LENGTH];
  const uint8_t* message = (const uint8_t *)payload;
  uint16_t i;
  COMMS_DEBUG("%u bytes from ", length);
  node_id = comnet::network::NodeByAddr(ieee);
  if(node_id != NULL) {
    COMMS_DEBUG("'%s':\n", node_id->node_info);
  } else {
    COMMS_DEBUG("%s:\n", addr64_format(buffer, ieee));
    COMMS_DEBUG("THIS XBEE SOURCE IS UNKNOWN!!\n");
  }
  for(i = 0; i < length && isprint(message[i]); ++i);
  if(i == length) {
    // all characters of message are printable
    COMMS_DEBUG("\t%.*s\n", length, message);
    XBeeMessage* m = new XBeeMessage;
    std::memcpy(m->message, message, length);
    m->length = length;
    message_queue.push_back(std::move(std::unique_ptr<XBeeMessage>(m)));
  } else {
    hex_dump(message, length, HEX_DUMP_FLAG_TAB);
  }
}


// function receiving data on transparent serial cluster when ATAO != 0
int TransparentRx(const wpan_envelope_t* envelope, void* context) {
  TransparentDump(&envelope->ieee_address, envelope->payload,
                   envelope->length);
  return 0;
}


// function receiving data on transparent serial cluster when ATAO=0
int ReceiveHandler(xbee_dev_t *xbee, const void* raw,
                    uint16_t length, void* context) {
  const xbee_frame_receive_t* rx_frame = (const xbee_frame_receive_t *)raw;
  if (length >= offsetof(xbee_frame_receive_t, payload)) {
    TransparentDump(&rx_frame->ieee_address, rx_frame->payload,
                     length - offsetof(xbee_frame_receive_t, payload));
  }
  return 0;
}


wpan_ep_state_t zdo_ep_state = { 0} ;


const wpan_cluster_table_entry_t digi_data_clusters[] = {
  // transparent serial goes here (cluster 0x0011)
  {DIGI_CLUST_SERIAL, TransparentRx, NULL,
  WPAN_CLUST_FLAG_INOUT | WPAN_CLUST_FLAG_NOT_ZCL},
  // handle join notifications (cluster 0x0095) when ATAO is not 0
  XBEE_DISC_DIGI_DATA_CLUSTER_ENTRY,

  WPAN_CLUST_ENTRY_LIST_END
};


const wpan_endpoint_table_entry_t sample_endpoints[] = {
  ZDO_ENDPOINT(zdo_ep_state),
  // Endpoint/cluster for transparent serial and OTA command cluster
  {WPAN_ENDPOINT_DIGI_DATA,		// endpoint
  WPAN_PROFILE_DIGI,				// profile ID
  NULL,									// endpoint handler
  NULL,									// ep_state
  0x0000,								// device ID
  0x00,									// version
  digi_data_clusters				// clusters
  },
  {WPAN_ENDPOINT_END_OF_LIST}
};

////////// end of endpoint table, only necessary if ATAO is not 0

const xbee_dispatch_table_entry_t xbee_frame_handlers[] = {
  XBEE_FRAME_HANDLE_LOCAL_AT,
  XBEE_FRAME_HANDLE_ATND_RESPONSE,		// for processing ATND responses
  // this entry is for when ATAO is not 0
  XBEE_FRAME_HANDLE_RX_EXPLICIT,		// rx messages via endpoint table
  // next two entries are used when ATAO is 0
  XBEE_FRAME_HANDLE_AO0_NODEID,			// for processing NODEID messages
  {XBEE_FRAME_RECEIVE, 0, ReceiveHandler, NULL},		// rx messages direct
  XBEE_FRAME_TABLE_END
};


namespace comnet {
namespace network {


using namespace experimental;


void NodeDiscovered(xbee_dev_t* xbee, const xbee_node_id_t* rec) {
  if (rec != NULL) {
    NodeAdd(rec);
    xbee_disc_node_id_dump(rec);
  }
}


XBee::XBee(const char* port, speed_t baudrate) 
{
  Initialize(port, baudrate);
}


XBee::~XBee() 
{
  CloseXBeePort();
}


// Initialize the xbee device.
bool XBee::Initialize(const char* port, speed_t baudrate) {
  bool success = false;
  int status;
#if COM_TARGET_OS == COM_OS_WINDOWS
  std::regex regularEx("\\b(COM)");
  std::string port_res;
  std::string port_str(port);
  std::regex_replace(std::back_inserter(port_res),
                     port_str.begin(), port_str.end(), regularEx, "$2");
  serial.comport = atoi(port_res.c_str());
  serial.hCom = NULL;
#else // POSIX stuff instead.
  strncpy(serial.device, port, (sizeof serial.device) - 1);
  serial.device[(sizeof serial.device) - 1] = '\0';
#endif 
  serial.baudrate = baudrate;
  // open the serial port to xbee.
  status = xbee_ser_open(&serial, baudrate);
  if (!status) { 
    xbee_ser_rx_flush(&serial);
    xbee_ser_tx_flush(&serial);
    if (xbee_dev_init(&device, &serial, NULL, NULL)) {
      COMMS_DEBUG("Xbee failed to initialize.\n");
    } else {
      xbee_wpan_init(&device, sample_endpoints);
      // Add node discover handler.
      xbee_disc_add_node_id_handler(&device, &NodeDiscovered);
      xbee_cmd_init_device(&device);
      COMMS_DEBUG("Waiting for driver to query Xbee device...\n");
      do {
        xbee_dev_tick(&device);
        status = xbee_cmd_query_status(&device);
      } while (status == -EBUSY);
      COMMS_DEBUG("Finished device xbee query...\n");
      if (!status) {
        xbee_dev_dump_settings(&device, XBEE_DEV_DUMP_FLAG_DEFAULT);
        // Discover xbee nodes.
        xbee_disc_discover_nodes(&device, NULL);
        success = true;
      } else {
        COMMS_DEBUG("Query status failed... Error code=%d\n", status);
      }
    }
  } else {
    COMMS_DEBUG("Failed to open serial port=%d\n", serial.comport);
  } 
  if (!success) {
    CloseXBeePort();
  }
  return success;
}


// Absolutely not sure this will work...
// Need to test this.
bool XBee::Recv(uint8_t* rxData, uint32_t* rxLength) {
  xbee_disc_discover_nodes(&device, NULL);
  bool success = false;
  if (!message_queue.empty()) {
    XBeeMessage* message = message_queue.front().get();
    memcpy(rxData, message->message, message->length);
    *rxLength = message->length;
    message_queue.pop_front();
  }
  return success;
}


bool XBee::Send(const char* wpan_addr, uint8_t* txData, uint32_t txLength) {
  xbee_disc_discover_nodes(&device, NULL);
  bool success = false;
  addr64 ieee_addr;
  int err = addr64_parse(&ieee_addr, wpan_addr);
  if (err != 0) {
    COMMS_DEBUG("Could not parse address effectively... error code=%d", err);
  } else {
    xbee_node_id_t* target;
    target = NodeByAddr(&ieee_addr);
    if (target != nullptr) {
      // perform data sending.
      wpan_envelope_t env;
      wpan_envelope_create(&env, &device.wpan_dev, &target->ieee_addr_be, target->network_addr);
      env.payload = txData;
      env.length = txLength;
      err = xbee_transparent_serial(&env);
      if (!err) { 
        COMMS_DEBUG("Sent: %d,\tover port=%d", env.length, serial.comport);
        success = true;
      } else {
        COMMS_DEBUG("Error sending packet! No packet sent...\n");
      }
    } else {
      COMMS_DEBUG("Target was not found! Could not send data!\n");
    }
  }
  return success;
}


bool XBee::CloseXBeePort() {
  bool success = false;
  if(!xbee_ser_close(&serial)) {
    COMMS_DEBUG("failed to close serial port=%d\n", serial.comport);
  } else {
    success = true;
  }
  return success;
}
} // namespace Network
} // namespace Comnet