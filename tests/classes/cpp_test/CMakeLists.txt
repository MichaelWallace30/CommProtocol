#
# Test main for testing.
#
cmake_minimum_required( VERSION 2.8 )
project(CPP_TEST)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../XbeeC/xbee_ansic_library/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../CommProto/include/)


add_executable(CommProtoExe
 ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.0)
  if (WIN32)
    target_compile_options(CommProtoExe PUBLIC /sdl-)
  else()
    target_compile_options(CommProtoExe PUBLIC -std=c++11 -pthreads)
  endif()
endif()


# Given that linux is different from windows
if(WIN32)
  target_link_libraries(CommProtoExe CommProtoStatic)
else()
  target_link_libraries(CommProtoExe CommProtoStatic)
endif()


message(${CMAKE_CXX_FLAGS})