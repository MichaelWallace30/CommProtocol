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
option(shared "Build shared/dll of the library" ON)
option(static "Build static/lib of the library" ON)
option(test "Build test executable to test library with" OFF)


# Setting up crypto++
add_subdirectory(Encryption/cryptopp564)


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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_PATH}/cmake/")

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
include(cmake/archfiles.cmake)
include(cmake/tools-config.cmake)
include(cmake/console.cmake)
include(cmake/CommProto-config.cmake)
include(cmake/hash-config.cmake)
include(cmake/network-config.cmake)
include(cmake/serialization-config.cmake)
include(cmake/pkg.cmake)
include(cmake/comnet_debug.cmake)
include(cmake/xbee_ansic.cmake)
include(cmake/encryption.cmake)

# contains the header/src filters for visual studio.
#
if (WIN32)
  include(cmake/visualc.cmake)
endif()

set(ENCRYPTION_LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/cryptopp564/debug)
link_directories(${ENCRYPTION_LIBDIR})

if (shared)
  # Shared Library (.so for linux or .dll for Windows).
  add_library(CommProtoShared SHARED ${NGCP_GLOB})
  target_link_libraries(CommProtoShared cryptopp-static)
  std_compiler_options(CommProtoShared)
endif()

if (static)
  # Static library (.a for linux or .lib for Windows).
  add_library(CommProtoStatic STATIC ${NGCP_GLOB})
  target_link_libraries(CommProtoStatic cryptopp-static)    
  std_compiler_options(CommProtoStatic)
endif()

if (test)
  # C++ testing.
  add_subdirectory(tests/classes/cpp_test)
  # Vistionary testing toolset.
  add_subdirectory(unit)
endif()

# Executable Unit Test
# add_executable(CommProtoExe
#  ${NGCP_GLOB}
#   CommProto/src/main.cpp)
