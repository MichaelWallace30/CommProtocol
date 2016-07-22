# CommProto configuration files

set(COMMPROTO_HEADERS
  ${PARENT_HEADER_DIR}/console/CommsDebug.h
  ${PARENT_HEADER_DIR}/comms.h  
  ${PARENT_HEADER_DIR}/AbstractPacket.h
  ${PARENT_HEADER_DIR}/Packets.h
  ${PARENT_HEADER_DIR}/CommsPacket.h
  ${PARENT_HEADER_DIR}/Callback.h)

set(COMMPROTO_SOURCES
  ${PARENT_SOURCE_DIR}/Packets.cpp
  ${PARENT_SOURCE_DIR}/AbstractPacket.cpp
  ${PARENT_SOURCE_DIR}/comms.cpp
  ${PARENT_SOURCE_DIR}/Callback.cpp)		


