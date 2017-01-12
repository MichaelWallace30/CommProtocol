# pkg configs
set(PING_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/ping/)
set(PING_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/ping/)

set(COMMPROTO_PING_HEADERS
  ${PING_HEADER_DIR}/pinger.h
  ${PING_HEADER_DIR}/ping_manager.h
  ${PING_HEADER_DIR}/ping_packet.h
  ${PING_HEADER_DIR}/sync_manager.h
  ${PING_HEADER_DIR}/sync_request_packet.h
  ${PING_HEADER_DIR}/sync_reply_packet.h)

set(COMMPROTO_PING_SOURCES
  ${PING_SOURCE_DIR}/pinger.cpp
  ${PING_SOURCE_DIR}/ping_manager.cpp
  ${PING_SOURCE_DIR}/ping_packet.cpp
  ${PING_SOURCE_DIR}/sync_manager.cpp
  ${PING_SOURCE_DIR}/sync_request_packet.cpp
  ${PING_SOURCE_DIR}/sync_reply_packet.cpp)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${COMMPROTO_PING_HEADERS}
  ${COMMPROTO_PING_SOURCES})