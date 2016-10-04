cmake_minimum_required( VERSION 2.8 )
project(CPP_TEST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../XbeeC/xbee_ansic_library/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../CommProto/include/)

add_executable(CommProtoExe
  main.cpp)


target_link_libraries(CommProtoExe CommProtoStatic)
target_link_libraries(CommProtoExe CommProtoShared)