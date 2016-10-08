# Please ensure your changes or patch meets minimum requirements.
#   The minimum requirements are below, and they are 2.8.5. Please
#   do not check in something for 2.8.12. To test your changes,
#   please set up a Ubuntu 12.04 LTS system. Then, manually install
#   Cmake 2.8.5 from http://cmake.org/Wiki/CMake_Released_Versions.

cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)


include(TestBigEndian)
#============================================================================
# Settable options
#============================================================================
option(BUILD_STATIC "Build static library" ON)
option(BUILD_SHARED "Build shared library" ON)
#============================================================================
# Internal compiler options
#============================================================================

# Don't use RPATH's. The resulting binary could fail a security audit.
if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
  set(CMAKE_MACOSX_RPATH 0)
endif()

# Endianess
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
	add_definitions(-DIS_BIG_ENDIAN)
endif()

if(NOT CRYPTOPP_DATA_DIR STREQUAL "")
	add_definitions(-DCRYPTOPP_DATA_DIR="${CRYPTOPP_DATA_DIR}")
endif()

#============================================================================
# Sources & headers
#============================================================================

# Library headers
file(GLOB cryptopp_HEADERS *.h)
# Library sources. You can use the GNUmakefile to generate the list: `make sources`.
file(GLOB cryptopp_SOURCES *.cpp)

list(REMOVE_ITEM cryptopp_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/simple.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib_bds.cpp
		${cryptopp_SOURCES_TEST}
		)
set(cryptopp_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${cryptopp_SOURCES}
		)

if(MINGW OR WIN32)
	list(APPEND cryptopp_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp)
endif()

if(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        list(APPEND cryptopp_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/x64dll.asm)
        list(APPEND cryptopp_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/x64masm.asm)
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/x64dll.asm PROPERTIES COMPILE_FLAGS "/D_M_X64")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/x64masm.asm PROPERTIES COMPILE_FLAGS "/D_M_X64")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/rdrand.asm PROPERTIES COMPILE_FLAGS "/D_M_X64")
    else()
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/x64dll.asm PROPERTIES COMPILE_FLAGS "/D_M_X86 /safeseh")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/x64masm.asm PROPERTIES COMPILE_FLAGS "/D_M_X86 /safeseh")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/rdrand.asm PROPERTIES COMPILE_FLAGS "/D_M_X86 /safeseh")
    endif()
    list(APPEND cryptopp_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rdrand.asm)
    enable_language(ASM_MASM)
endif()

#============================================================================
# Compile targets
#============================================================================

# Set global includes BEFORE adding any targets for legacy CMake versions
if(CMAKE_VERSION VERSION_LESS 2.8.12)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Set PIC
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# Enables -fPIC on all 64-bit platforms
    if (BUILD_STATIC)
        get_target_property(flags_old_static cryptopp-static COMPILE_FLAGS)
        if(NOT flags_old_static)
            set(flags_old_static "")
        endif()
        set_target_properties(cryptopp-static PROPERTIES COMPILE_FLAGS "${flags_old_static} -fPIC")
    endif()
    if (BUILD_SHARED)
        get_target_property(flags_old_shared cryptopp-shared COMPILE_FLAGS)
        if(NOT flags_old_shared)
            set(flags_old_shared "")
        endif()
        set_target_properties(cryptopp-shared PROPERTIES COMPILE_FLAGS "${flags_old_shared} -fPIC")
    endif()
endif()


#============================================================================
# Third-party libraries
#============================================================================
if(WIN32)
	if (BUILD_STATIC)
		target_link_libraries(cryptopp-static ws2_32)
	endif()
	if (BUILD_SHARED)
		target_link_libraries(cryptopp-shared ws2_32)
	endif()
endif()

find_package(Threads)
if (BUILD_STATIC)
	target_link_libraries(cryptopp-static ${CMAKE_THREAD_LIBS_INIT})
endif()
if (BUILD_SHARED)
	target_link_libraries(cryptopp-shared ${CMAKE_THREAD_LIBS_INIT})
endif()

if (BUILD_STATIC)
	add_library(cryptopp-static STATIC ${cryptopp_SOURCES})
endif()

if (BUILD_SHARED)
	add_library(cryptopp-shared SHARED ${cryptopp_SOURCES})
endif()


