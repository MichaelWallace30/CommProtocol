#include <CommProto/network/XBee.h>

#include <CommProto/architecture/macros.h>

// xbee includes.
#include <xbee/platform.h>
#include <xbee/commissioning.h>
#include <xbee/device.h>
#include <xbee/wpan.h>

#include <zigbee/zcl_commissioning.h>
#include <zigbee/zcl_identify.h>

uint16_t profile_id = 0x0105;		// ZBA


const zcl_comm_startup_param_t zcl_comm_default_sas =
{
  0xFFFE,											// short_address
  ZCL_COMM_GLOBAL_EPID,						// extended_panid
  0xFFFF,											// panid (0xFFFF = not joined)
  UINT32_C(0x7FFF) << 11,						// channel_mask
  0x02,												// startup_control
                              /*
                              { { 0, 0, 0, 0, 0, 0, 0, 0 } },			// trust_ctr_addr
                              { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                              // trust_ctr_master_key
                              */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  // network_key
  FALSE,											// use_insecure_join
                              //	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 'C', 'a', 'l', '0', 'P', 'o', 'l', 'y', '0', 'P', 'o', 'm', 'o', 'n', 'a', '0' },
  // preconfig_link_key
  0x00,												// network_key_seq_num
  ZCL_COMM_KEY_TYPE_STANDARD,				// network_key_type
  0x0000,											// network_mgr_addr

  ZCL_COMM_SCAN_ATTEMPTS_DEFAULT,			// scan_attempts
  ZCL_COMM_TIME_BETWEEN_SCANS_DEFAULT,	// time_between_scans
  ZCL_COMM_REJOIN_INTERVAL_DEFAULT,		// rejoin_interval
  ZCL_COMM_MAX_REJOIN_INTERVAL_DEFAULT,	// max_rejoin_interval

  {
    FALSE,											// concentrator.flag
    ZCL_COMM_CONCENTRATOR_RADIUS_DEFAULT,	// concentrator.radius
    0x00												// concentrator.discovery_time
  },
};


namespace Comnet {
namespace Network {


using namespace Experimental;

XBee::XBee(const char* port, speed_t baudrate) 
{
  serial.baudrate = baudrate;
  
}
} // namespace Network
} // namespace Comnet