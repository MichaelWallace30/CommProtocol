# pkg configs
set(PKG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/pkg/)
set(PKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/pkg/)

set(COMMPROTO_PKG_HEADERS
  ${PKG_HEADER_DIR}/PacketManager.h
  ${PKG_HEADER_DIR}/PacketTable.h
  ${PKG_HEADER_DIR}/PacketFactory.h)

set(COMMPROTO_PKG_SOURCES
  ${PKG_SOURCE_DIR}/PacketManager.cc
  ${PKG_SOURCE_DIR}/PacketTable.cc)