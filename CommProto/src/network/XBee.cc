/*
XBee Link that handles all xbee connections.

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
#include <CommProto/network/XBee.h>
#include <CommProto/network/zigbee_configs.h>
#include <CommProto/debug/CommsDebug.h>
#include <CommProto/architecture/macros.h>
#include <CommProto/HeaderPacket.h>

#include <regex>
#include <string>
#include <ctype.h>



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
      xbee_dev_dump_settings(&device, XBEE_DEV_DUMP_FLAG_DEFAULT);
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