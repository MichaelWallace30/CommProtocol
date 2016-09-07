# CommProto configuration files

set(COMMPROTO_HEADERS
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


