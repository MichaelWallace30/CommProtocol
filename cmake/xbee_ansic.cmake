# Xbee Ansic Library builds.
include_directories(SYSTEM XbeeC/xbee_ansic_library/include/)

set(XBEE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/XbeeC/xbee_ansic_library/include/)
set(XBEE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/XbeeC/xbee_ansic_library/src/)


# Header for the xbee library. Glob it!
set(XBEE_HEADERS
  ${XBEE_HEADER_DIR}/wpan/aps.h
  ${XBEE_HEADER_DIR}/wpan/types.h

  ${XBEE_HEADER_DIR}/xbee/atcmd.h
  ${XBEE_HEADER_DIR}/xbee/atmode.h
  ${XBEE_HEADER_DIR}/xbee/byteorder.h
  ${XBEE_HEADER_DIR}/xbee/cbuf.h
#  ${XBEE_HEADER_DIR}/xbee/commissioning.h
  ${XBEE_HEADER_DIR}/xbee/device.h
  ${XBEE_HEADER_DIR}/xbee/discovery.h
  ${XBEE_HEADER_DIR}/xbee/ebl_file.h
  ${XBEE_HEADER_DIR}/xbee/io.h
  ${XBEE_HEADER_DIR}/xbee/jslong.h
  ${XBEE_HEADER_DIR}/xbee/jslong_glue.h
  ${XBEE_HEADER_DIR}/xbee/ota_client.h
  ${XBEE_HEADER_DIR}/xbee/ota_server.h
  ${XBEE_HEADER_DIR}/xbee/platform.h
  ${XBEE_HEADER_DIR}/xbee/reg_descr.h
  ${XBEE_HEADER_DIR}/xbee/route.h
  ${XBEE_HEADER_DIR}/xbee/serial.h
  ${XBEE_HEADER_DIR}/xbee/sxa.h
  ${XBEE_HEADER_DIR}/xbee/time.h
  ${XBEE_HEADER_DIR}/xbee/transparent_serial.h
  ${XBEE_HEADER_DIR}/xbee/wpan.h
  ${XBEE_HEADER_DIR}/xbee/xmodem.h
  ${XBEE_HEADER_DIR}/xbee/xmodem_crc16.h)


set(ZIGBEE_HEADERS
  ${XBEE_HEADER_DIR}/zigbee/zcl.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_bacnet.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_basic.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_basic_attributes.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_client.h
#  ${XBEE_HEADER_DIR}/zigbee/zcl_commissioning.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_identify.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_onoff.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_time.h
  ${XBEE_HEADER_DIR}/zigbee/zcl_types.h
  ${XBEE_HEADER_DIR}/zigbee/zcl64.h
  ${XBEE_HEADER_DIR}/zigbee/zdo.h)

# Globbin' the xbee sources.
set(XBEE_SOURCES
  ${XBEE_SOURCE_DIR}/util/hexdump.c
  ${XBEE_SOURCE_DIR}/util/hexstrtobyte.c
  ${XBEE_SOURCE_DIR}/util/jslong.c
  ${XBEE_SOURCE_DIR}/util/memcheck.c
  ${XBEE_SOURCE_DIR}/util/swapbytes.c
  ${XBEE_SOURCE_DIR}/util/swapcpy.c
  ${XBEE_SOURCE_DIR}/util/xmodem_crc16.c

  ${XBEE_SOURCE_DIR}/xbee/xbee_atcmd.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_atmode.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_cbuf.c
#  ${XBEE_SOURCE_DIR}/xbee/xbee_commissioning.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_device.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_discovery.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_ebl_file.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_io.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_ota_client.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_ota_server.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_reg_descr.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_route.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_sxa.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_time.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_transparent_serial.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_wpan.c
  ${XBEE_SOURCE_DIR}/xbee/xbee_xmodem.c

  ${XBEE_SOURCE_DIR}/wpan/wpan_aps.c
  ${XBEE_SOURCE_DIR}/wpan/wpan_types.c)

set(ZIGBEE_SOURCES
  ${XBEE_SOURCE_DIR}/zigbee/zcl_basic.c
  ${XBEE_SOURCE_DIR}/zigbee/zcl_client.c
#  ${XBEE_SOURCE_DIR}/zigbee/zcl_commissioning.c
  ${XBEE_SOURCE_DIR}/zigbee/zcl_identify.c
  ${XBEE_SOURCE_DIR}/zigbee/zcl_onoff.c
  ${XBEE_SOURCE_DIR}/zigbee/zcl_time.c
  ${XBEE_SOURCE_DIR}/zigbee/zcl_types.c
  ${XBEE_SOURCE_DIR}/zigbee/zigbee_zcl.c
  ${XBEE_SOURCE_DIR}/zigbee/zigbee_zdo.c)

if (WIN32)

  set(XBEE_HEADERS
    ${XBEE_HEADERS}
    ${ZIGBEE_HEADERS}
    ${XBEE_HEADER_DIR}/xbee/platform_win32.h)

  set(XBEE_SOURCES
    ${XBEE_SOURCES}
    ${ZIGBEE_SOURCES}
    ${XBEE_SOURCE_DIR}/win32/xbee_platform_win32.c
    ${XBEE_SOURCE_DIR}/win32/xbee_serial_win32.c)


elseif(UNIX)


  set(XBEE_HEADERS
    ${XBEE_HEADERS}
    ${ZIGBEE_HEADERS}
    ${XBEE_HEADER_DIR}/xbee/platform_posix.h)

  set(XBEE_SOURCES
    ${XBEE_SOURCES}
    ${ZIGBEE_SOURCES}
    ${XBEE_SOURCE_DIR}/posix/xbee_platform_posix.c
    ${XBEE_SOURCE_DIR}/posix/xbee_serial_posix.c
    ${XBEE_SOURCE_DIR}/posix/xbee_readline.c)
endif()


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${XBEE_HEADERS}
  ${XBEE_SOURCES})