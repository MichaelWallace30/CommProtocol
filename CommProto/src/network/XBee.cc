#include <CommProto/network/XBee.h>
#include <CommProto/debug/CommsDebug.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/HeaderPacket.h>

// xbee includes.
#include <xbee/platform.h>
#include <xbee/atcmd.h>
#include <xbee/commissioning.h>
#include <xbee/wpan.h>
#include <xbee/cbuf.h>
#include <xbee/serial.h>

#include <zigbee/zcl_commissioning.h>
#include <zigbee/zcl_client.h>
#include <zigbee/zcl_identify.h>
#include <zigbee/zcl_types.h>
#include <zigbee/zdo.h>

#include <regex>
#include <string>
#include <ctype.h>

uint16_t profile_id = 0x0105;		// ZBA


#define SAMPLE_COMMISION_ENDPOINT 1

typedef struct basic_value_t {
  uint8_t	app_ver;
  uint8_t	stack_ver;
  char		model_id[33];
  char		datecode[17];
} basic_value_t;

typedef struct target_t
{
  addr64			ieee;
  uint8_t			endpoint;
  basic_value_t	basic;
} target_t;

#define TARGET_COUNT 40
extern target_t target_list[TARGET_COUNT];
extern int target_index;				// number of targets in target_list

struct _endpoints {
  wpan_endpoint_table_entry_t		zdo;
  wpan_endpoint_table_entry_t		zcl;
  uint8_t									end_of_list;
};

extern struct _endpoints sample_endpoints;
int xbee_found(wpan_conversation_t FAR *conversation,
               const wpan_envelope_t FAR *envelope);

void print_target(int index);


#define ZCL_APP_VERSION			0x12
#define ZCL_MANUFACTURER_NAME	"Digi International"
#define ZCL_MODEL_IDENTIFIER	"ZCL Commissioning Client"
#define ZCL_DATE_CODE			"20110331 dev"
#define ZCL_POWER_SOURCE		ZCL_BASIC_PS_SINGLE_PHASE
#include "zigbee/zcl_basic_attributes.h"

                                /// Used to track ZDO transactions in order to match responses to requests
                                /// (#ZDO_MATCH_DESC_RESP).
wpan_ep_state_t zdo_ep_state = {0};

/// Tracks state of ZCL endpoint.
wpan_ep_state_t zcl_ep_state = {0};

basic_value_t basic_value;

/*
It would be interesting to explore a way to store offsets instead of
addresses in this structure, and then copy it to the stack while converting
offsets to an actual address (either in an array, or a temp object on the
stack).
*/
const struct {
  zcl_attribute_base_t		app_ver;
  zcl_attribute_base_t		stack_ver;
  zcl_attribute_full_t		model_id;
  zcl_attribute_full_t		datecode;
} basic_attr = {
  {ZCL_BASIC_ATTR_APP_VERSION,
  ZCL_ATTRIB_FLAG_NONE,
  ZCL_TYPE_UNSIGNED_8BIT,
  &basic_value.app_ver,
  },
  {ZCL_BASIC_ATTR_STACK_VERSION,
  ZCL_ATTRIB_FLAG_NONE,
  ZCL_TYPE_UNSIGNED_8BIT,
  &basic_value.stack_ver,
  },
  {
    {ZCL_BASIC_ATTR_MODEL_IDENTIFIER,
    ZCL_ATTRIB_FLAG_FULL,
    ZCL_TYPE_STRING_CHAR,
    basic_value.model_id,
    },
    {0}, {sizeof(basic_value.model_id) - 1},
    NULL, NULL
  },
  {
    {ZCL_BASIC_ATTR_DATE_CODE,
    ZCL_ATTRIB_FLAG_FULL,
    ZCL_TYPE_STRING_CHAR,
    basic_value.datecode,
    },
    {0}, {sizeof(basic_value.datecode) - 1},
    NULL, NULL
  },
};

target_t target_list[TARGET_COUNT];
int target_index = 0;

int find_target(const addr64 *ieee)
{
  uint8_t i;

  for(i = 0; i < target_index; ++i)
  {
    if(addr64_equal(&target_list[i].ieee, ieee))
    {
      // found in list
      return i;
    }
  }

  return -1;
}

void print_target(int index)
{
  target_t *t;

  t = &target_list[index];
  if(!addr64_is_zero(&t->ieee))
  {
    printf("%2u: %08" PRIx32 "-%08" PRIx32 " 0x%02x %-32s %-16s\n", index,
           be32toh(t->ieee.l[0]), be32toh(t->ieee.l[1]),
           t->basic.app_ver, t->basic.model_id, t->basic.datecode);
  }
}

                                // parse the response to reading the other device's Basic Attributes
int basic_parse(zcl_command_t *zcl)
{
  uint8_t			response = ZCL_STATUS_SUCCESS;
  int				i;

  i = find_target(&zcl->envelope->ieee_address);
  if(i >= 0)
  {
    // could ignore duplicates by checking (target_list[i].basic.app_ver > 0)

    memset(&basic_value, 0, sizeof basic_value);

    response = zcl_process_read_attr_response(zcl, &basic_attr.app_ver);

    target_list[i].basic = basic_value;
    print_target(i);
  }

  return zcl_default_response(zcl, response);
}

int basic_client(const wpan_envelope_t FAR *envelope,
                 void FAR *context)
{
  zcl_command_t	zcl;

  if(zcl_command_build(&zcl, envelope, (zcl_attribute_tree_t *)context) == 0 &&
     zcl.command == ZCL_CMD_READ_ATTRIB_RESP &&
     ZCL_CMD_MATCH(&zcl.frame_control, GENERAL, SERVER_TO_CLIENT, PROFILE))
  {
    // response to our read basic attr
    return basic_parse(&zcl);
  }

  return zcl_general_command(envelope, context);
}

int commissioning_client(const wpan_envelope_t FAR *envelope,
                         void FAR *context)
{
  const char *cmd = NULL;
  zcl_command_t	zcl;
  int err;

  puts("frame for commissioning client:");

  err = zcl_command_build(&zcl, envelope, (zcl_attribute_tree_t *)context);
  if(err)
  {
    printf("\tzcl_command_build() returned %d for %u-byte payload\n",
           err, envelope->length);
    hex_dump(envelope->payload, envelope->length, HEX_DUMP_FLAG_TAB);
  } else
  {
    if(ZCL_CMD_MATCH(&zcl.frame_control,
       GENERAL, SERVER_TO_CLIENT, CLUSTER))
    {
      switch(zcl.command)
      {
      case ZCL_COMM_CMD_RESTART_DEVICE_RESP:
      cmd = "Restart Device Response";
      break;

      case ZCL_COMM_CMD_SAVE_STARTUP_PARAM_RESP:
      cmd = "Save Startup Param Response";
      break;

      case ZCL_COMM_CMD_RESTORE_STARTUP_PARAM_RESP:
      cmd = "Restore Startup Param Response";
      break;

      case ZCL_COMM_CMD_RESET_STARTUP_PARAM_RESP:
      cmd = "Reset Startup Param Response";
      break;
      }

      // pretty-print properly formatted commands that we know
      if(cmd != NULL && zcl.length == 1)
      {
        printf("\t%s: Status = %s\n", cmd,
               zcl_status_text(*(const uint8_t FAR *)zcl.zcl_payload));

        // ignore this response
        return zcl_default_response(&zcl, ZCL_STATUS_SUCCESS);
      }
    }

    // if not already parsed, dump the raw command
    zcl_command_dump(&zcl);
  }

  // let the ZCL General Command handler process all unhandled frames
  return zcl_general_command(envelope, context);
}

const wpan_cluster_table_entry_t zcl_cluster_table[] =
{
  {ZCL_CLUST_BASIC,
  &basic_client,
  zcl_basic_attribute_tree,
  WPAN_CLUST_FLAG_INOUT
  },

  {ZCL_CLUST_COMMISSIONING,
  &commissioning_client,
  NULL,
  WPAN_CLUST_FLAG_CLIENT
  },

  WPAN_CLUST_ENTRY_LIST_END
};

struct _endpoints sample_endpoints = {
  ZDO_ENDPOINT(zdo_ep_state),

  {SAMPLE_COMMISION_ENDPOINT,		// endpoint
  0,										// profile ID (filled in later)
  NULL,									// endpoint handler
  &zcl_ep_state,						// ep_state
  0x0000,								// device ID
  0x00,									// version
  zcl_cluster_table,				// clusters
  },

  WPAN_ENDPOINT_END_OF_LIST
};


const zcl_comm_startup_param_t zcl_comm_default_sas =
{
  0xFFFE,	                    // short_address
  ZCL_COMM_GLOBAL_EPID,       // extended_panid
  0xFFFF,	                    // panid (0xFFFF = not joined)
  UINT32_C(0x7FFF) << 11,						// channel_mask
  0x02,                     // startup_control
                              /*
                              { { 0, 0, 0, 0, 0, 0, 0, 0 } },			// trust_ctr_addr
                              { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                              // trust_ctr_master_key
                              */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  // network_key
  FALSE,                      // use_insecure_join
                              //	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 'C', 'a', 'l', '0', 'P', 'o', 'l', 'y', '0', 'P', 'o', 'm', 'o', 'n', 'a', '0' },
  // preconfig_link_key
  0x00,                         // network_key_seq_num
  ZCL_COMM_KEY_TYPE_STANDARD,   // network_key_type
  0x0000,	                      // network_mgr_addr

  ZCL_COMM_SCAN_ATTEMPTS_DEFAULT,	    // scan_attempts
  ZCL_COMM_TIME_BETWEEN_SCANS_DEFAULT,  // time_between_scans
  ZCL_COMM_REJOIN_INTERVAL_DEFAULT,	  // rejoin_interval
  ZCL_COMM_MAX_REJOIN_INTERVAL_DEFAULT,	// max_rejoin_interval

  {
    FALSE,  // concentrator.flag
    ZCL_COMM_CONCENTRATOR_RADIUS_DEFAULT,	// concentrator.radius
    0x00  // concentrator.discovery_time
  },
};

const xbee_dispatch_table_entry_t xbee_frame_handlers[] =
{
  XBEE_FRAME_HANDLE_LOCAL_AT,
  XBEE_FRAME_TABLE_END
};

zcl_comm_startup_param_t network_deploy =
{
  0xFFFE,											// short_address
  ZCL64_INIT(0, 0x0123),						// extended_panid
  0xFFFF,											// panid (0xFFFF = not joined)
  UINT32_C(0x7FFF) << 11,						// channel_mask
  ZCL_COMM_STARTUP_JOINED,					// startup_control
                                    /*
                                    { { 0, 0, 0, 0, 0, 0, 0, 0 } },			// trust_ctr_addr
                                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                    // trust_ctr_master_key
                                    */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
  // network_key
  FALSE,											// use_insecure_join
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'M', 'S', 'E', 'D', 'C', 'L'},
  // preconfig_link_key
  0x00,												// network_key_seq_num
  ZCL_COMM_KEY_TYPE_STANDARD,				// network_key_type
  0x0000,											// network_mgr_addr

  ZCL_COMM_SCAN_ATTEMPTS_DEFAULT,			// scan_attempts
  ZCL_COMM_TIME_BETWEEN_SCANS_DEFAULT,	// time_between_scans
  ZCL_COMM_REJOIN_INTERVAL_DEFAULT,		// rejoin_interval
  ZCL_COMM_MAX_REJOIN_INTERVAL_DEFAULT,	// max_rejoin_interval

  {FALSE,											// concentrator.flag
  ZCL_COMM_CONCENTRATOR_RADIUS_DEFAULT,	// concentrator.radius
  0x00												// concentrator.discovery_time
  },
};

namespace Comnet {
namespace Network {


using namespace Experimental;

XBee::XBee(const char* port, speed_t baudrate) 
{
  initialize(port, baudrate);
}


XBee::~XBee() 
{
  closePort();
}


// Initialize the xbee device.
bool XBee::initialize(const char* port, speed_t baudrate) {
  bool success = false;
  int status;
#if COM_TARGET_OS == COM_OS_WINDOWS
  std::regex regularEx("\\b(COM)");
  std::string portResult;
  std::string portString(port);
  std::regex_replace(std::back_inserter(portResult),
                     portString.begin(), portString.end(), regularEx, "$2");
  serial.comport = atoi(portResult.c_str());
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
      xbee_wpan_init(&device, &sample_endpoints.zdo);
      xbee_cmd_init_device(&device);
      COMMS_DEBUG("Waiting for driver to query Xbee device...\n");
      do {
        xbee_dev_tick(&device);
        status = xbee_cmd_query_status(&device);
      } while (status == -EBUSY);
      if (!status) {
        xbee_dev_dump_settings(&device, XBEE_DEV_DUMP_FLAG_DEFAULT);
        sample_endpoints.zcl.profile_id = profile_id;
        success = true;
      }
    }
  } else {
    COMMS_DEBUG("Failed to open serial port=%d", serial.comport);
  } 
  return success;
}


bool XBee::recv(uint8_t* rxData, uint32_t* rxLength) {
  bool success = false;
  int read_success = xbee_ser_read(&serial, (void *)rxData, MAX_BUFFER_SIZE);
  if (read_success >= 0) {
    *rxLength = read_success;
    success = true;
  } else {
    COMMS_DEBUG("Error in reading xbee...\b");
  }
  return success;
}


bool XBee::send(uint8_t destId, uint8_t* txData, uint32_t txLength) {
  bool success = false;
  int send_success = xbee_ser_write(&serial, (void *)txData, txLength);
  if (send_success >= 0) {
    COMMS_DEBUG("Sent: %d,\tover port=%d", send_success, serial.comport);
    success = true;
  }
  return success;
}


bool XBee::closePort() {
  bool success = false;
  if(!xbee_ser_close(&serial)) {
    COMMS_DEBUG("failed to close serial port=%d", serial.comport);
  } else {
    success = true;
  }
  return success;
}
} // namespace Network
} // namespace Comnet