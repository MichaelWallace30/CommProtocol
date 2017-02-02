# pkg configs
set(CONSTATE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/constate/)
set(CONSTATE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/constate/)

set(COMMPROTO_CONSTATE_HEADERS
  ${CONSTATE_HEADER_DIR}/check_connect_reply.h
  ${CONSTATE_HEADER_DIR}/check_connect_request.h
  ${CONSTATE_HEADER_DIR}/connection_state.h
  ${CONSTATE_HEADER_DIR}/connection_state_manager.h
  ${CONSTATE_HEADER_DIR}/time_sync_manager.h
  ${CONSTATE_HEADER_DIR}/time_sync_reply.h
  ${CONSTATE_HEADER_DIR}/time_sync_request.h)

set(COMMPROTO_CONSTATE_SOURCES
  ${CONSTATE_SOURCE_DIR}/connection_state.cpp
  ${CONSTATE_SOURCE_DIR}/connection_state_manager.cpp
  ${CONSTATE_SOURCE_DIR}/time_sync_manager.cpp)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${COMMPROTO_CONSTATE_HEADERS}
  ${COMMPROTO_CONSTATE_SOURCES})