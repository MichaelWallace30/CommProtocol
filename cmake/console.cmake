# intercommunications for various processes and programs.

set(CONSOLE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/console/)
set(CONSOLE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/console/)

set(CONSOLE_HEADERS
  ${CONSOLE_HEADER_DIR}/code.h
  ${CONSOLE_HEADER_DIR}/console.h
  ${CONSOLE_HEADER_DIR}/parse_engine.h
  ${CONSOLE_HEADER_DIR}/piping.h)

# Console is not active yet.
set(CONSOLE_SOURCES
  ${CONSOLE_SOURCE_DIR}/console.cc)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${CONSOLE_HEADERS}
  ${CONSOLE_SOURCES})