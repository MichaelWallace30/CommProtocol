cmake_minimum_required(VERSION 2.8)
project(CPP_TEST)

set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../CommProto/tools/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)

set(COMMPROTO_TOOLS_HEADERS
  ${TOOLS_DIR}/data_structures/AutoQueue.h
  ${TOOLS_DIR}/data_structures/SingleLinkedList.h
  ${TOOLS_DIR}/data_structures/LinkedQueue.h
  ${TOOLS_DIR}/data_structures/DoubleLinkedList.h
  ${TOOLS_DIR}/data_structures/interface/Generic.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceList.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceQueue.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceMap.h
  ${TOOLS_DIR}/data_structures/interface/InterfaceTree.h
  ${TOOLS_DIR}/allocator/NewAllocator.h
  ${TOOLS_DIR}/allocator/CommPointer.h
  ${TOOLS_DIR}/Comparable.h
  ${TOOLS_DIR}/Comparator.h
  ${TOOLS_DIR}/StandardComparator.h
  ${TOOLS_DIR}/hash/NgcpHash.h)

set(COMMPROTO_TOOLS_SOURCES
  ${TOOLS_DIR}/hash/NgcpHash.cpp)

add_executable(TESTING_MAIN
  ${COMMPROTO_TOOLS_HEADERS}
  ${COMMPROTO_TOOLS_SOURCES}
  main.cpp)