cmake_minimum_required( VERSION  2.8 )
 
project( Unit_Test )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread" )

get_filename_component(TEST_DIR ${CMAKE_SOURCE_DIR}/../. DIRECTORY)

include_directories(AFTER SYSTEM ../ )

set (NGCP_UNIT
	../architecture/os/linux-config/linux-config.h 
	../architecture/os/include_defines.h
	../tools/Comparator.h
	../tools/data_structures/interface/Generic.h
	../tools/data_structures/interface/InterfaceList.h
	../tools/data_structures/interface/InterfaceQueue.h
	${TEST_DIR}/tools/data_structures/LinkedQueue.h
	../tools/data_structures/SingleLinkedList.h
	../unit/Test.cpp )

add_executable( UNIT ${NGCP_UNIT} )
