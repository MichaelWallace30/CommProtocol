cmake_minimum_required( VERSION 2.8 )
project ("CommProtoCSharp")

include_directories(SYSTEM ../../CommProto/include/)
include_directories(SYSTEM ../../XbeeC/xbee_ansic_library/include/)
include_directories(SYSTEM ../../Encryption/cryptopp564/)
include_directories(SYSTEM .)

# Remove Run Time Checks from this build
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /sdl-") 
string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
string(REPLACE "/RTC1" "/clr" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

set(CSHARP_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set(COMMPROTO_WRAPPER_HEADER_DIR 
  ${CSHARP_DIR}/tools/data_structures/interface/Queue.h
  ${CSHARP_DIR}/tools/data_structures/AutoQueue.h
  ${CSHARP_DIR}/network/TransportProtocol.h
  ${CSHARP_DIR}/network/CommsLink.h
  ${CSHARP_DIR}/network/TCPLink.h
  ${CSHARP_DIR}/network/UDPLink.h
  ${CSHARP_DIR}/network/SerialLink.h
  ${CSHARP_DIR}/network/XbeeLink.h
  ${CSHARP_DIR}/pkg/PacketManager.h
  ${CSHARP_DIR}/pkg/PacketTable.h
  ${CSHARP_DIR}/pkg/PacketFactory.h
  ${CSHARP_DIR}/constate/ConnectionStateManager.h
  ${CSHARP_DIR}/constate/ConnectionState.h
  ${CSHARP_DIR}/constate/CheckConnectReply.h
  ${CSHARP_DIR}/constate/CheckConnectRequest.h
  ${CSHARP_DIR}/constate/TimeSyncManager.h
  ${CSHARP_DIR}/constate/TimeSyncReply.h
  ${CSHARP_DIR}/constate/TimeSyncRequest.h
  ${CSHARP_DIR}/CommNode.h
  ${CSHARP_DIR}/Comms.h
  ${CSHARP_DIR}/Header.h
  ${CSHARP_DIR}/ObjectStream.h
  ${CSHARP_DIR}/CallBack.h
  ${CSHARP_DIR}/ABSPacket.h
  ${CSHARP_DIR}/ObjSerializable.h)

set(COMMPROTO_WRAPPER_SOURCE_DIR
  ${CSHARP_DIR}/network/TCPLink.cpp
  ${CSHARP_DIR}/network/UDPLink.cpp
  ${CSHARP_DIR}/network/SerialLink.cpp
  ${CSHARP_DIR}/network/XbeeLink.cpp
  ${CSHARP_DIR}/tools/data_structures/AutoQueue.cc
  ${CSHARP_DIR}/pkg/PacketTable.cc
  ${CSHARP_DIR}/pkg/PacketManager.cc
  ${CSHARP_DIR}/constate/ConnectionStateManager.cc
  ${CSHARP_DIR}/constate/ConnectionState.cc
  ${CSHARP_DIR}/constate/TimeSyncManager.cc
  ${CSHARP_DIR}/CommNode.cc
  ${CSHARP_DIR}/Comms.cc
  ${CSHARP_DIR}/Header.cpp
  ${CSHARP_DIR}/ObjectStream.cpp
  ${CSHARP_DIR}/CallBack.cc
  ${CSHARP_DIR}/ABSPacket.cc)
  


add_library(CommProtoCSharp SHARED
  ${COMMPROTO_WRAPPER_HEADER_DIR}
  ${COMMPROTO_WRAPPER_SOURCE_DIR})

target_link_libraries(CommProtoCSharp CommProtocol-Static)
target_link_libraries(CommProtoCSharp cryptopp-static)
