# Tools config files, which are added into a set for compilation
set(TOOLS_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/tools/)
set(TOOLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/tools/)

set(TOOLS_HEADERS
  ${TOOLS_HEADER_DIR}/observer.h
  ${TOOLS_HEADER_DIR}/comparable.h
  ${TOOLS_HEADER_DIR}/comparator.h
  ${TOOLS_HEADER_DIR}/standard_comparator.h
  ${TOOLS_HEADER_DIR}/data_structures/rbtree.h
  ${TOOLS_HEADER_DIR}/data_structures/btree.h
  ${TOOLS_HEADER_DIR}/data_structures/auto_queue.h
  ${TOOLS_HEADER_DIR}/data_structures/auto_vector.h
  ${TOOLS_HEADER_DIR}/data_structures/circle_linked_list.h
  ${TOOLS_HEADER_DIR}/data_structures/double_linked_list.h
  ${TOOLS_HEADER_DIR}/data_structures/single_linked_list.h
  ${TOOLS_HEADER_DIR}/data_structures/linked_queue.h
  ${TOOLS_HEADER_DIR}/data_structures/thread_safe_list.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/generic.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/interface_list.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/interface_queue.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/interface_map.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/interface_tree.h
  ${TOOLS_HEADER_DIR}/data_structures/interface/interface_stack.h
  ${TOOLS_HEADER_DIR}/allocator/allocator.h
  ${TOOLS_HEADER_DIR}/allocator/commpointer.h
  ${TOOLS_HEADER_DIR}/allocator/newallocator.h
  ${TOOLS_HEADER_DIR}/allocator/stackallocator.h)

set(TOOLS_SOURCES
	)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${TOOLS_HEADERS}
  ${TOOLS_SOURCES})