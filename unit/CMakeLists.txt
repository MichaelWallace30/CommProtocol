cmake_minimum_required( VERSION  3.0 )
 
project( Unit_Test )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread" )

include_directories(AFTER SYSTEM ../ )

set (NGCP_UNIT 
	../architecture/os/include_defines.h
	../tools/Comparator.hpp
	../unit/Test.cpp )

add_executable( UNIT ${NGCP_UNIT} )
