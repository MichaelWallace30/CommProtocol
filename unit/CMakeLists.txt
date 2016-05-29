cmake_minimum_required( VERSION  2.8 )
 
project( Unit_Test )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread" )

include_directories(AFTER SYSTEM ../ )

set (NGCP_UNIT
	../architecture/os/linux-config/linux-config.h 
	../architecture/os/include_defines.h
	../tools/Comparator.h
	../tools/data_structures/interface/Generic.h
	../tools/data_structures/interface/InterfaceList.h
	../tools/data_structures/SingleLinkedList.h
	../unit/Test.cpp )

add_executable( UNIT ${NGCP_UNIT} )
