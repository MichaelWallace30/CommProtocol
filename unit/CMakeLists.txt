project( Unit_Test )


set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread" )

include_directories(SYSTEM ./ )

set (NGCP_UNIT 
	tools/Comparator.hpp
	unit/Test.cpp )

add_executable( UNIT ${NGCP_UNIT} )
