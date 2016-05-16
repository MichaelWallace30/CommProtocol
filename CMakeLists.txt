cmake_minimum_required( VERSION 3.0 )

project( "NGCP-Comnet-v2" )

get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY )

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

include_directories(SYSTEM ./)

set(SOURCES
	architecture/os/osThreads.h
	architecture/os/linux-config/linux-config.h
	architecture/os/thread-config.h
	"architecture/os/osThreads.c"
	CommProto/AbstractPacket.hpp
	CommProto/AbstractPacket.cpp
	CommProto/comms.h
	CommProto/comms.cpp
	CommProto/commsLink.h
	CommProto/commsLink.cpp
	CommProto/CommsPacket.h
	CommProto/main.cpp)

set( TEST_SRCS "commProto/main.cpp" )		

add_executable( NGCP_TEST ${SOURCES} )
