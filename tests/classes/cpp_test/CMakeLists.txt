#
# Test main for testing.
#
cmake_minimum_required( VERSION 2.8 )
project(CPP_TEST)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../XbeeC/xbee_ansic_library/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../CommProto/include/)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../../../Encryption/cryptopp564/)


add_executable(CommProtoExe
 ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

if (CMAKE_VERION VERSION_GREATER 2.8.11)
  target_compile_options(CommProtoExe PUBLIC -std=c++11)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread")
else()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11 -pthread")
endif()
# Given that linux is different from windows
target_link_libraries(CommProtoExe CommProtoStatic)
target_link_libraries(CommProtoExe cryptopp-static)
