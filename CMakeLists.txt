cmake_minimum_required( VERSION 2.8 )

project( "NGCP-Comnet-v2" )

# define a few variables.

# Set options at build time. These options will allow the user to build the file in which ever 
# bit version they need.
# Be sure to call in the cmake file: -Dbuild32=ON for 32bit build (enabled by default)
#
#                                    -Dbuild64=ON for 64bit build (will disable build32 build)
#
# If you plan on changing build versions after making changes, such as changing the build from 64 to 32,
# you must specify -Dbuild64=OFF -Dbuild32=ON along with the build. You can instead just use -Dbuild64=ON (or OFF) to 
# switch between building 64bit or 32bit.
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
  message(FATAL_ERROR "Operating system architecture is not compatible with this software. Exiting build.")
  return()
endif()

# Set the compiler build options.
# Will determine the architecture that this library will be built in.
if (build64)
  message("CommProtocol library 64-bit compile. x86-64 build enabled...")
  set(build32 OFF)
else()
  message("CommProtocol library 32-bit compile. x86 build enabled...")
  set(build32 ON)
  set(build64 OFF)
endif()

# Check for possible problems that may occur in the operating system's architecture.
if (build32)
  if (${ARCH_DIR} STREQUAL "32-bit")
    message("32-bit build will commence...")
  else()
    message("NOTICE: This operating system supports both 64 and 32 bit applications...")
    message(WARNING "WARNING: Before compiling, be sure you have 32-bit version of your compiler's libraries.")
  endif()
  set(COMPILER_BUILD "-m32")
elseif (build64)
  if (${ARCH_DIR} STREQUAL "64-bit")
    message("64-bit build will commence...")
  else()
    message("NOTICE: This operating system is not able to run 64-bit libraries and applications. However, build will continue as 64-bit...")
    message(WARNING "WARNING: Before compiling, be sure you have 32-bit version of your compiler's libraries.")
  endif()
  set(COMPILER_BUILD "-m64")
else()
  message(FATAL_ERROR "Attempting to build both 64 and 32 bit builds. Build will now exit.")
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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_PATH}/cmake/")
set(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/)
set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/tools/)


# Add some commands to the compiler.
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} -std=c++11 -pthread")

# Don't forget to include the directories for the compiler to check for.
include_directories(SYSTEM ./)

# Our sources, which will be included in the build. This is a test build, to 
# check for problems and situations arising from our code base. We will most likely
# debug using an IDE such as Visual Studio for Windows, and possibly NewBeans or Eclipse 
# or linux and OS X.
include(cmake/archfiles.cmake)

set(COMMPROTO_TOOLS_HEADERS
  ${TOOLS_DIR}/data_structures/AutoQueue.h
  ${TOOLS_DIR}/data_structures/SingleLinkedList.h
  ${TOOLS_DIR}/data_structures/LinkedQueue.h
  ${TOOLS_DIR}/data_structures/DoubleLinkedList.h
  ${TOOLS_DIR}/data_structures/interface/Generic.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceList.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceQueue.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceMap.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceTree.h
  ${TOOLS_DIR}/allocator/NewAllocator.h
  ${TOOLS_DIR}/allocator/CommPointer.h
  ${TOOLS_DIR}/Comparable.h
  ${TOOLS_DIR}/Comparator.h
  ${TOOLS_DIR}/StandardComparator.h
  ${TOOLS_DIR}/hash/NgcpHash.h)

set(COMMPROTO_TOOLS_SOURCES
  ${TOOLS_DIR}/hash/NgcpHash.cpp)

set(COMMPROTO_HEADERS
  ${PARENT_DIR}/comms.h
  ${PARENT_DIR}/network/CommsLink.h
  ${PARENT_DIR}/AbstractPacket.h
  ${PARENT_DIR}/CommsPacket.h
  ${PARENT_DIR}/network/UDP.h)

set(COMMPROTO_SOURCES
  ${PARENT_DIR}/AbstractPacket.cpp
  ${PARENT_DIR}/comms.cpp
  ${PARENT_DIR}/network/UDP.cpp
  ${PARENT_DIR}/network/CommsLink.cpp)		

add_executable(NGCP_LIB 
  ${ARCHITECTURE_HEADERS}
  ${ARCHITECTURE_SOURCES}
  ${COMMPROTO_HEADERS}
  ${COMMPROTO_SOURCES}
  ${COMMPROTO_TOOLS_HEADERS}
  ${COMMPROTO_TOOLS_SOURCES})
