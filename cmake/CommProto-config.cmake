# CommProto configuration files
set(PARENT_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/)
set(PARENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/)

set(COMMPROTO_HEADERS
  ${PARENT_HEADER_DIR}/CommNode.h
  ${PARENT_HEADER_DIR}/Comms.h  
  ${PARENT_HEADER_DIR}/AbstractPacket.h
  ${PARENT_HEADER_DIR}/Packets.h
  ${PARENT_HEADER_DIR}/HeaderPacket.h
  ${PARENT_HEADER_DIR}/Callback.h)

set(COMMPROTO_SOURCES
  ${PARENT_SOURCE_DIR}/Packets.cpp
  ${PARENT_SOURCE_DIR}/AbstractPacket.cpp
  ${PARENT_SOURCE_DIR}/HeaderPacket.cc
  ${PARENT_SOURCE_DIR}/Comms.cpp
  ${PARENT_SOURCE_DIR}/Callback.cpp)		


