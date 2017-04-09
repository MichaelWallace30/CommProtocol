####################################################################################
#  Main CMake script for which to run the build                                    #
#                                                                                  #
#  Copyright(C) 2016  Alex Craig, Michael Wallace, Mario Garcia.                   #
#                                                                                  #
#  This program is free software : you can redistribute it and / or modify         #
#  it under the terms of the GNU General Public License as published by            #
#  the Free Software Foundation, either version 3 of the License, or               #
#  (At your option) any later version.                                             #
#                                                                                  #
#  This program is distributed in the hope that it will be useful,                 #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of                  #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                    #
#  GNU General Public License for more details.                                    #
#                                                                                  #
#  You should have received a copy of the GNU General Public License               #
#  along with this program.If not, see <http://www.gnu.org/licenses/>.             #
#                                                                                  #
####################################################################################
cmake_minimum_required( VERSION 2.8 )

project( "CommProtocol" )

# Commprotocol Versioning
#

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
option(shared "Build shared/dll of the library" OFF)
option(static "Build static/lib of the library" ON)
option(test "Build test executable to test library with" OFF)
option(csharp "Build C# DLL for .NET use" OFF)
# Options to allow debug/release version of CommProtocol.
option(version_release "Compile the release version of CommProtocol" OFF)


if (version_release)
  message("Setting to release.")
  set (CMAKE_BUILD_TYPE Release)
  add_definitions(-DCOMMPROTO_RELEASE)
else()
  set (CMAKE_BUILD_TYPE Debug) 
  add_definitions(-DCOMMPROTO_DEBUG)
endif()

# Adding definitions externally.
# works essentially the same. 
add_definitions(
  -DCOMMPROTO_CURRENT_VERSION=0x108000
  -DCOMMPROTO_COMPATIBLE_VERSION=0x106000
  -DCOMMPROTO_MINIMUM_VERSION=0x100000
)

# Setting up crypto++
add_subdirectory(Encryption/cryptopp563)
#setyp libxbee3
add_subdirectory(libxbee3)


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
  set(OS_SYSTEM "unix")
elseif (WIN32) 
  set(OS_SYSTEM "win32")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OS_SYSTEM "os x")
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

# Add macros for build
include(cmake/macros.cmake)

# Add the cmake module path, for module build paths for this cmake file.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Don't forget to include the directories for the compiler to check for.
include_directories(SYSTEM CommProto/include/)


# check global compiler options.
check_global_compiler_options()

# Set NGCP_GLOB to be our file cluster to build.
set (NGCP_GLOB)

# Our sources, which will be included in the build. This is a test build, to 
# check for problems and situations arising from our code base. We will most likely
# debug using an IDE such as Visual Studio for Windows, and possibly NewBeans or Eclipse 
# or linux and OS X.
include(archfiles)
include(tools-config)
include(console)
include(CommProto-config)
include(hash-config)
include(network-config)
include(serialization-config)
include(constate)
include(pkg)
include(comnet_debug)
# include(xbee_ansic) # We are no longer using this library.
include(encryption)

# contains the header/src filters for visual studio.
#
if (WIN32)
  include(cmake/visualc.cmake)
endif()


#link cryptopp dependencies directory
if (CXX_COMPILER MATCHES "MSVC")
	set(ENCRYPTION_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/cryptopp563/debug)
    set(LIBXBEE3_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/libxbee3/debug)
else()
	set(ENCRYPTION_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/cryptopp563)
    set(LIBXBEE3_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/libxbee3)
endif()
link_directories(${ENCRYPTION_LIBDIR})
link_directories(${LIBXBEE3_LIBDIR})


if (shared)
  # Shared Library (.so for linux or .dll for Windows).
  add_library(CommProtocol-Shared SHARED ${NGCP_GLOB})
  target_compile_options(CommProtocol-Shared PUBLIC -fPIC)
  target_compile_definitions(CommProtocol-Shared  PUBLIC COMMPROTO_DLL)
  target_link_libraries(CommProtocol-Shared cryptopp-static)
  target_link_libraries(CommProtocol-Shared libxbee3)
  std_compiler_options(CommProtocol-Shared)
endif()

if (static)
  # Static library (.a for linux or .lib for Windows).
  add_library(CommProtocol-Static STATIC ${NGCP_GLOB})
  target_link_libraries(CommProtocol-Static cryptopp-static) 
  target_link_libraries(CommProtocol-Static libxbee3)  
  std_compiler_options(CommProtocol-Static)
endif()

if (test)
  # C++ testing.
  add_subdirectory(tests/classes/cpp/udp/)
  add_subdirectory(tests/classes/cpp/tcp/)
  add_subdirectory(tests/classes/cpp/xbee/)
  add_subdirectory(tests/classes/cpp/serial/)
  # Vistionary testing toolset.
  add_subdirectory(unit)
endif()

if (csharp)
  # C# DLL building
  if (WIN32)
    message("C# DLL is building...")
    add_subdirectory(modules/csharp/)
  else()
    message(WARNING "Can not build C# .NET DLL on this platform. Skipping this module...")
  endif()
endif()

# Executable Unit Test
# add_executable(CommProtoExe
#  ${NGCP_GLOB}
#   CommProto/src/main.cpp)
