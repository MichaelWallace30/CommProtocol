# intercommunications for various processes and programs.

set(CONSOLE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/console/)
set(CONSOLE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/console/)

set(CONSOLE_HEADERS)
set(CONSOLE_SOURCES)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${CONSOLE_HEADERS}
  ${CONSOLE_SOURCES})