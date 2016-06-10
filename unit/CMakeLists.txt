cmake_minimum_required( VERSION  2.8 )
 
project( Unit_Test )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread" )

# get_filename_component(TEST_DIR ${CMAKE_SOURCE_DIR}/../. DIRECTORY)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../CommProto/include/CommProto)

include_directories(AFTER SYSTEM ../CommProto/include/ )

set (NGCP_UNIT
	${TEST_DIR}/architecture/os/linux-config/linux-config.h 
	${TEST_DIR}/architecture/os/include_defines.h
	${TEST_DIR}/tools/Comparator.h
	${TEST_DIR}/tools/data_structures/interface/Generic.h
	${TEST_DIR}/tools/data_structures/interface/InterfaceList.h
	${TEST_DIR}/tools/data_structures/interface/InterfaceQueue.h
	${TEST_DIR}/tools/data_structures/LinkedQueue.h
	${TEST_DIR}/architecture/macros.h
	${TEST_DIR}/architecture/api.h
	${TEST_DIR}/tools/data_structures/SingleLinkedList.h
	${TEST_DIR}/tools/data_structures/DoubleLinkedList.h
	${TEST_DIR}/tools/data_structures/AutoVector.h
	Test.cpp )

add_executable( UNIT ${NGCP_UNIT} )
