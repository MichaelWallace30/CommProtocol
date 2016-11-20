# pkg configs
set(PKG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/pkg/)
set(PKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/pkg/)

set(COMMPROTO_PKG_HEADERS
  ${PKG_HEADER_DIR}/packetmanager.h
  ${PKG_HEADER_DIR}/packettable.h
  ${PKG_HEADER_DIR}/packethashtable.h
  ${PKG_HEADER_DIR}/packetfactory.h)

set(COMMPROTO_PKG_SOURCES
  ${PKG_SOURCE_DIR}/packetmanager.cc
  ${PKG_SOURCE_DIR}/packethashtable.cpp)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${COMMPROTO_PKG_HEADERS}
  ${COMMPROTO_PKG_SOURCES})