cmake_minimum_required( VERSION 2.8 )

project( "NGCP-Comnet-v2" )


# define a few variables.


# Check the architecture variable. If the Architecture is 64 bit, we define a variable to state that we
# are working with a 64-bit operating system architecture. 32-bit will also be defined as well.
# Anything lower will not be compatible with this software. 
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH_DIR "64-bit")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
 	set(ARCH_DIR "32-bit")
else()
	message("Operating system architecture is not compatible with this software. Exiting build.")
	return()
endif()


# Check the Operating system so that we have a clear target of what we are working with.
# From here, we can determine how to build this library without too much work in the source
# files, however, we can simply do the operating system checking using the preprocessor defines.
if (UNIX)
  set(OS_SYSTEM "Unix")
elseif (WIN32) 
  set(OS_SYSTEM "win32")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OS_SYSTEM "OS X")
endif(UNIX)


# We define the compiler to determine what we are working with, and what files we need to add to the build in 
# order to add dependencies.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CXX_COMPILER "Clang")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(CXX_COMPILER "MSVC")
elseif (CMAKE_COMPILER_IS_GNUCXX)
  set(CXX_COMPILER "g++")
elseif (MINGW) 
  set(CXX_COMPILER "MinGW")
else()
  message("This compiler was not detected by the cmake build, so it may not be compatible with this software. However, this build will continue.")
endif()




get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY )

# Add some commands to the compiler.
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

# Don't forget to include the directories for the compiler to check for.
include_directories(SYSTEM ./)

# Our sources, which will be included in the build. This is a test build, to 
# check for problems and situations arising from our code base. We will most likely
# debug using an IDE such as Visual Studio for Windows, and possibly NewBeans or Eclipse 
# or linux and OS X.
set(SOURCES
  architecture/os/osThreads.h
  architecture/os/linux-config/linux-config.h
  architecture/os/thread-config.h
  "architecture/os/osThreads.c"
  CommProto/AbstractPacket.h
  CommProto/AbstractPacket.cpp
  CommProto/comms.h
  CommProto/comms.cpp
  CommProto/commsLink.h
  CommProto/commsLink.cpp
  CommProto/CommsPacket.h
  CommProto/main.cpp)

set( TEST_SRCS "commProto/main.cpp" )		

add_executable( NGCP_TEST ${SOURCES} )
