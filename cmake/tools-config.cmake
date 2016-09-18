# Tools config files, which are added into a set for compilation
set(TOOLS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/tools/)
set(TOOLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/tools/)

set(TOOLS_HEADERS
  ${TOOLS_HEADER_DIR}/Comparable.h
  ${TOOLS_HEADER_DIR}/Comparator.h
  ${TOOLS_HEADER_DIR}/StandardComparator.h
  ${TOOLS_HEADER_DIR}/data_structures/AutoQueue.h
  ${TOOLS_HEADER_DIR}/data_structures/AutoVector.h
  ${TOOLS_HEADER_DIR}/data_structures/CircleLinkedList.h
  ${TOOLS_HEADER_DIR}/data_structures/DoubleLinkedList.h
  ${TOOLS_HEADER_DIR}/data_structures/SingleLinkedList.h
  ${TOOLS_HEADER_DIR}/data_structures/LinkedQueue.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/Generic.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/InterfaceList.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/InterfaceQueue.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/InterfaceMap.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/InterfaceTree.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/InterfaceStack.h
  ${TOOLS_HEADER_DIR}/allocator/Allocator.h
  ${TOOLS_HEADER_DIR}/allocator/CommPointer.h
  ${TOOLS_HEADER_DIR}/allocator/NewAllocator.h
  ${TOOLS_HEADER_DIR}/allocator/StackAllocator.h)

set(TOOLS_SOURCES
	)