# Architecture files defined and configured in this .cmake file.
set(ARCHITECTURE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/architecture/)
set(ARCHITECTURE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/architecture/)

set(ARCHITECTURE_HEADERS
  ${ARCHITECTURE_HEADER_DIR}/os/linux-config/linux-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/include_defines.h
  ${ARCHITECTURE_HEADER_DIR}/os/arch.h
  ${ARCHITECTURE_HEADER_DIR}/os/os_threads.h
  ${ARCHITECTURE_HEADER_DIR}/os/thread-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/mutex-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/os_mutex.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_mutex.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_thread.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_semaphore.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_monitor.h
  ${ARCHITECTURE_HEADER_DIR}/api.h
  ${ARCHITECTURE_HEADER_DIR}/macros.h
  ${ARCHITECTURE_HEADER_DIR}/connection/socket-config.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial_status.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config_win32.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config_linux.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config.h
  ${ARCHITECTURE_HEADER_DIR}/connection/transport_type.h)

set(ARCHITECTURE_SOURCES
  ${ARCHITECTURE_SOURCE_DIR}/os/comm_monitor.cc
  ${ARCHITECTURE_SOURCE_DIR}/os/comm_semaphore.cc
  ${ARCHITECTURE_SOURCE_DIR}/os/comm_thread.cc
  ${ARCHITECTURE_SOURCE_DIR}/os/comm_mutex.cc
  ${ARCHITECTURE_SOURCE_DIR}/os/os_threads.cpp
  ${ARCHITECTURE_SOURCE_DIR}/os/os_mutex.cpp)
