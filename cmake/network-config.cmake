# Tools config files, which are added into a set for compilation
set(NETWORK_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/network/)
set(NETWORK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/network/)


set(NETWORK_HEADERS
  ${NETWORK_HEADER_DIR}UDP.h
  ${NETWORK_HEADER_DIR}Serial.h
  ${NETWORK_HEADER_DIR}/SerialLink.h
  ${NETWORK_HEADER_DIR}/XBee.h
  ${NETWORK_HEADER_DIR}/XBeeLink.h
  ${NETWORK_HEADER_DIR}/UDPLink.h
  ${NETWORK_HEADER_DIR}CommSocket.h
  ${NETWORK_HEADER_DIR}CommsLink.h
  ${NETWORK_HEADER_DIR}Crc32.h
  ${NETWORK_HEADER_DIR}Parser.h)

set(NETWORK_SOURCES
  ${NETWORK_SOURCE_DIR}UDP.cpp
  ${NETWORK_SOURCE_DIR}Serial.cpp
  ${NETWORK_SOURCE_DIR}/SerialLink.cc
  ${NETWORK_SOURCE_DIR}/XBee.cc
  ${NETWORK_SOURCE_DIR}/XBeeLink.cc
  ${NETWORK_SOURCE_DIR}/UDPLink.cc
  ${NETWORK_SOURCE_DIR}TcpSocket.cpp
  ${NETWORK_SOURCE_DIR}Crc32.cpp
  ${NETWORK_SOURCE_DIR}Parser.cpp)