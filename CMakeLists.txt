cmake_minimum_required( VERSION 3.0 )

project( "NGCP-Comnet-v2" )

get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY )

include_directories(SYSTEM ./)

set( TEST_SRCS "commProto/main.cpp" )		

add_executable( NGCP_TEST ${TEST_SRCS} )
