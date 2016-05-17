cmake_minimum_required( VERSION 3.0 )

project( "NGCP-Comnet-v2" )


# define a few variables.


# Check the architecture variable 
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH_DIR "64-bit")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
 	set(ARCH_DIR "32-bit")
else()
	message("Operating system architecture is not compatible with this software. Exiting build.")
	return()
endif()




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
