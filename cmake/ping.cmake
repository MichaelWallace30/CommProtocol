# pkg configs
set(PING_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/ping/)
set(PING_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/ping/)

set(COMMPROTO_PING_HEADERS
  ${PING_HEADER_DIR}/pinger.h
  ${PING_HEADER_DIR}/pingmanager.h
  ${PING_HEADER_DIR}/pingpacket.h)

set(COMMPROTO_PING_SOURCES
  ${PING_SOURCE_DIR}/pinger.cpp
  ${PING_SOURCE_DIR}/pingmanager.cpp
  ${PING_SOURCE_DIR}/pingpacket.cpp)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${COMMPROTO_PING_HEADERS}
  ${COMMPROTO_PING_SOURCES})