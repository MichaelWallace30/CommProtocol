
cmake_minimum_required(VERSION 3.0)
if (CMAKE_VERSION VERSION_GREATER 3.0.2 )
  cmake_policy(SET CMP0054 OLD)
endif()

project("libxbee3")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#needs pthreads and probably c++11 for g++
#set(CMAKE_CXX_FLAS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

add_library(libxbee3 SHARED
conn.h
frame.h
internal.h
ll.h
log.h
mode.h
mutex.h
pkt.h
prepare.h
rx.h
thread.h
tx.h
xbee.h
xbeep.h
xbee_int.h
xsys.h
modes/common.h
modes/xbee5/at.h
modes/xbee5/data.h
modes/xbee5/dataExp.h
modes/xbee5/identify.h
modes/xbee5/io.h
modes/xbee5/mode5.h
conn.c
error.c
frame.c
ll.c
log.c
mode.c
mutex.c
pkt.c
prepare.c
rx.c
thread.c
tx.c
ver.c
xbee.c
xsys.c
modes/common.c
modes/xbee5/at.c
modes/xbee5/data.c
modes/xbee5/dataExp.c
modes/xbee5/identify.c
modes/xbee5/io.c
modes/xbee5/mode5.c)

if (test)
	if(WIN32 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	  set(COPY_CMD echo f | xcopy)
	  set(COPY_OPTION /y /R)
	  set(DYNLIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug)
      set(DYNLIB_NAME libxbee3.dll)
	  set(USE_DYNLIB TRUE)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	  set(COPY_CMD cp)
	  set(DYNLIB_DIR ${CMAKE_CURRENT_BINARY_DIR})
      set(DYNLIB_NAME libxbee3.dylib)
	  set(USE_DYNLIB TRUE)
	else()
	  set(USE_DYNLIB FALSE)
	endif()
	if (USE_DYNLIB)
	  add_custom_command(TARGET libxbee3
	    POST_BUILD
	    COMMAND cd \"${DYNLIB_DIR}\"
	    COMMAND ${COPY_CMD} \"${DYNLIB_NAME}\" \"${CMAKE_CURRENT_BINARY_DIR}/../tests/classes/cpp/udp/${DYNLIB_NAME}\" ${COPY_OPTION}
	    COMMAND ${COPY_CMD} \"${DYNLIB_NAME}\" \"${CMAKE_CURRENT_BINARY_DIR}/../tests/classes/cpp/xbee/${DYNLIB_NAME}\" ${COPY_OPTION}
	    COMMAND ${COPY_CMD} \"${DYNLIB_NAME}\" \"${CMAKE_CURRENT_BINARY_DIR}/../tests/classes/cpp/serial/${DYNLIB_NAME}\" ${COPY_OPTION}
		COMMAND ${COPY_CMD} \"${DYNLIB_NAME}\" \"${CMAKE_CURRENT_BINARY_DIR}/../tests/classes/cpp/tcp/${DYNLIB_NAME}\" ${COPY_OPTION})
	endif()
endif()