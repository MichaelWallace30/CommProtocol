# CommProto configuration files

set(COMMPROTO_HEADERS
  ${PARENT_HEADER_DIR}/console/CommsDebug.h
  ${PARENT_HEADER_DIR}/comms.h
  ${PARENT_HEADER_DIR}/network/CommsLink.h
  ${PARENT_HEADER_DIR}/AbstractPacket.h
  ${PARENT_HEADER_DIR}/CommsPacket.h
  ${PARENT_HEADER_DIR}/network/UDP.h
  ${PARENT_HEADER_DIR}/network/Serial.h
  ${PARENT_HEADER_DIR}/network/CommSocket.h)

set(COMMPROTO_SOURCES
  ${PARENT_SOURCE_DIR}/AbstractPacket.cpp
  ${PARENT_SOURCE_DIR}/comms.cpp
  ${PARENT_SOURCE_DIR}/network/UDP.cpp
  ${PARENT_SOURCE_DIR}/network/Serial.cpp
  ${PARENT_SOURCE_DIR}/network/TcpSocket.cpp)		


