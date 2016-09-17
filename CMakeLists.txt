cmake_minimum_required( VERSION 2.8 )

project( "NGCP-Comnet-v2" )

# Must set this policy to interpret if() arguments as variables and/or kewords when unquoted.
# Must check if the version is greater than 3.0.2 to use CMP0054 OLD policy
if (CMAKE_VERSION VERSION_GREATER 3.0.2 )
  cmake_policy(SET CMP0054 OLD)
endif()
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
option(build32 "Build library in 32 bit" OFF)
option(build64 "Build library in 64 bit" ON)

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

set(PARENT_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/)
set(PARENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/)
set(HASH_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/hash/)
set(HASH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/hash/)
set(TOOLS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/tools/)
set(TOOLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/tools/)
set(TOOLS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/tools/)
set(TOOLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/tools/)
set(NETWORK_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/network/)
set(NETWORK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/network/)
set(SERIALIZATION_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/serialization/)
set(SERIALIZATION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/serialization/)
set(PKG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/pkg/)
set(PKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/pkg/)
set(DEBUG_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/debug/)

# Add some commands to the compiler.
if (CXX_COMPILER MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} /sdl-")
else()
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} -std=c++11 -pthread")
endif()

# Don't forget to include the directories for the compiler to check for.
include_directories(SYSTEM CommProto/include/)

# Our sources, which will be included in the build. This is a test build, to 
# check for problems and situations arising from our code base. We will most likely
# debug using an IDE such as Visual Studio for Windows, and possibly NewBeans or Eclipse 
# or linux and OS X.
include(cmake/archfiles.cmake)
include(cmake/tools-config.cmake)
include(cmake/CommProto-config.cmake)
include(cmake/hash-config.cmake)
include(cmake/network-config.cmake)
include(cmake/serialization-config.cmake)
include(cmake/pkg.cmake)
include(cmake/comnet_debug.cmake)

if (${CMAKE_CXX_COMPILER_ID})    
    source_group("Header Files\\architecture" FILES ${ARCHITECTURE_HEADERS})
    source_group("Header Files\\hash" FILES ${COMMPROTO_HASH_HEADERS})
    source_group("Header Files\\tools" FILES ${TOOLS_HEADERS})
    source_group("Header Files\\network" FILES ${NETWORK_HEADERS})
    source_group("Header Files\\serialization" FILES ${SERIALIZATION_HEADERS})
    source_group("Header Files\\pkg" FILES ${COMMPROTO_PKG_HEADERS})
    source_group("Header Files\\debug" FILES ${DEBUG_HEADERS})
    source_group("Source Files\\architecture" FILES ${ARCHITECTURE_SOURCES})
    source_group("Source Files\\hash" FILES ${COMMPROTO_HASH_SOURCES})
    source_group("Source Files\\network" FILES ${NETWORK_SOURCES})
    source_group("Source Files\\serialization" FILES ${SERIALIZATION_SOURCES})
    source_group("Source Files\\pkg" FILES ${COMMPROTO_PKG_SOURCES})
endif()

set (NGCP_GLOB
  ${ARCHITECTURE_HEADERS}
  ${ARCHITECTURE_SOURCES}
  ${COMMPROTO_HEADERS}
  ${COMMPROTO_SOURCES}
  ${COMMPROTO_HASH_HEADERS}
  ${COMMPROTO_HASH_SOURCES}
  ${TOOLS_HEADERS}
  ${TOOLS_SOURCES}
  ${NETWORK_HEADERS}
  ${NETWORK_SOURCES}
  ${SERIALIZATION_HEADERS}
  ${SERIALIZATION_SOURCES}
  ${COMMPROTO_PKG_HEADERS}
  ${COMMPROTO_PKG_SOURCES}
  ${DEBUG_HEADERS})

# Shared Library (.so for linux or .dll for Windows).
add_library(CommProtoShared SHARED ${NGCP_GLOB})


# Static library (.a for linux or .lib for Windows).
add_library(CommProtoStatic  STATIC ${NGCP_GLOB})


# This doesn't do anything really...
target_link_libraries(CommProtoStatic CommProtoShared)

# Executable Unit Test
add_executable(CommProtoExe
  ${NGCP_GLOB}
   CommProto/src/main.cpp)