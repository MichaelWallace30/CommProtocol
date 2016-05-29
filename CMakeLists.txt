cmake_minimum_required( VERSION 2.8 )

project( "NGCP-Comnet-v2" )


# define a few variables.

# Set options at build time. These options will allow the user to build the file in which ever 
# bit version they need.
# Be sure to call in the cmake file: -Dbuild32=ON for 32bit build (enabled by default)
#
#                                    -Dbuild54=ON for 64bit build (will disable build32 build)
option(build32 "Build library in 32 bit" ON)
option(build64 "Build library in 64 bit" OFF)

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

# Set the compiler build options.
# Will determine the architecture that this library will be built in.
if (build64)
  message("CommProtocol library 64-bit compile. x86-64 build enabled...")
  set(build32 OFF)
endif()

if (build32 AND NOT build64)
  set(COMPILER_BUILD "-m32")
elseif (build64 AND NOT build32)
  if (ARCH_DIR NOT STREQUAL "64-bit")
    message("NOTICE: This operating system does not support 64-bit applications, however, built will continue as 32-bit")
  endif()
  set(COMPILER_BUILD "-m64")
else()
  message(FATAL_ERROR "Attempting to build 64 and 32 bit builds. Build will now exit.")
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

# Add the cmake module path, for module build paths for this cmake file.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOUCE_PATH}/cmake/")




get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY )

# Add some commands to the compiler.
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} -std=c++11 -pthread")

# Don't forget to include the directories for the compiler to check for.
include_directories(SYSTEM ./)

# Our sources, which will be included in the build. This is a test build, to 
# check for problems and situations arising from our code base. We will most likely
# debug using an IDE such as Visual Studio for Windows, and possibly NewBeans or Eclipse 
# or linux and OS X.
set(ARCHITECTURE_HEADERS 
  architecture/os/linux-config/linux-config.h
  architecture/os/include_defines.h
  architecture/os/arch.h
  architecture/os/os_threads.h
  architecture/os/thread-config.h
  architecture/os/mutex-config.h
  architecture/os/os_mutex.h)

set(ARCHITECTURE_SOURCES
  architecture/os/os_threads.c
  architecture/os/os_mutex.c)

set(COMMPROTO_HEADERS
  CommProto/comms.h
  CommProto/commsLink.h
  CommProto/AbstractPacket.h
  CommProto/CommsPacket.h)

set(COMMPROTO_SOURCES
  CommProto/AbstractPacket.cpp
  CommProto/comms.cpp
  CommProto/commsLink.cpp
  CommProto/main.cpp)		

add_executable( NGCP_TEST 
  ${ARCHITECTURE_HEADERS}
  ${ARCHITECTURE_SOURCES}
  ${COMMPROTO_HEADERS}
  ${COMMPROTO_SOURCES})
