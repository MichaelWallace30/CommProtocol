cmake_minimum_required( VERSION 2.8 )
project ("CommProtoCSharp")

include_directories(SYSTEM ../../CommProto/include/)
include_directories(SYSTEM ../../XbeeC/xbee_ansic_library/include/)
include_directories(SYSTEM .)

# Remove Run Time Checks from this build
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /sdl-") 
string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(REPLACE "/RTC1" "/clr" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

set(CSHARP_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set(COMMPROTO_WRAPPER_HEADER_DIR 
  ${CSHARP_DIR}/network/TransportProtocol.h
  ${CSHARP_DIR}/pkg/CPacketManager.h
  ${CSHARP_DIR}/pkg/CPacketTable.h
  ${CSHARP_DIR}/pkg/CPacketFactory.h
  ${CSHARP_DIR}/CCommNode.h
  ${CSHARP_DIR}/CommsWrapper.h
  ${CSHARP_DIR}/CHeader.h
  ${CSHARP_DIR}/CObjectStream.h
  ${CSHARP_DIR}/CallBack.h
  ${CSHARP_DIR}/ABSPacket.h)

set(COMMPROTO_WRAPPER_SOURCE_DIR
  ${CSHARP_DIR}/pkg/CPacketFactory.cc
  ${CSHARP_DIR}/pkg/CPacketTable.cc
  ${CSHARP_DIR}/pkg/CPacketManager.cc
  ${CSHARP_DIR}/CommsWrapper.cpp
  ${CSHARP_DIR}/CHeader.cpp
  ${CSHARP_DIR}/CObjectStream.cpp
  ${CSHARP_DIR}/CallBack.cc
  ${CSHARP_DIR}/ABSPacket.cc)

add_library(CommProtoCSharp SHARED
  ${COMMPROTO_WRAPPER_HEADER_DIR}
  ${COMMPROTO_WRAPPER_SOURCE_DIR})

target_link_libraries(CommProtoCSharp CommProtoStatic)

