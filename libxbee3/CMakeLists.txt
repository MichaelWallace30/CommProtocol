
cmake_minimum_required(VERSION 3.0)
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
