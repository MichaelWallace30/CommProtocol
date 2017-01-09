# CommProto configuration files
set(PARENT_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/)
set(PARENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/)

set(COMMPROTO_HEADERS
  ${PARENT_HEADER_DIR}/commproto.h
  ${PARENT_HEADER_DIR}/commnode.h
  ${PARENT_HEADER_DIR}/comms.h  
  ${PARENT_HEADER_DIR}/abstractpacket.h
  ${PARENT_HEADER_DIR}/headerpacket.h
  ${PARENT_HEADER_DIR}/callback.h)

set(COMMPROTO_SOURCES
  ${PARENT_SOURCE_DIR}/abstractpacket.cpp
  ${PARENT_SOURCE_DIR}/headerpacket.cc
  ${PARENT_SOURCE_DIR}/comms.cpp
  ${PARENT_SOURCE_DIR}/callback.cpp)		



set (NGCP_GLOB
  ${NGCP_GLOB}
  ${COMMPROTO_HEADERS}
  ${COMMPROTO_SOURCES})