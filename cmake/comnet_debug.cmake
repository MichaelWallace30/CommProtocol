# Debug files defined and configured in this .cmake file
set(DEBUG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/debug/)
set(DEBUG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/debug/)

set(DEBUG_HEADERS
  ${DEBUG_HEADER_DIR}/comms_debug.h
  ${DEBUG_HEADER_DIR}/error_status.h
  ${DEBUG_HEADER_DIR}/log.h)

set(DEBUG_SOURCES
  ${DEBUG_SOURCE_DIR}/log.cc)

set (NGCP_GLOB
  ${NGCP_GLOB}
  ${DEBUG_HEADERS}
  ${DEBUG_SOURCES})