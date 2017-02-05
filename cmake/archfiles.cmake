# Architecture files defined and configured in this .cmake file.
set(ARCHITECTURE_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/architecture/)
set(ARCHITECTURE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/architecture/)

set(ARCHITECTURE_HEADERS
  ${ARCHITECTURE_HEADER_DIR}/api.h
  ${ARCHITECTURE_HEADER_DIR}/macros.h
  ${ARCHITECTURE_HEADER_DIR}/cond.h
  ${ARCHITECTURE_HEADER_DIR}/win32_pipes.h
  ${ARCHITECTURE_HEADER_DIR}/posix_pipes.h
  ${ARCHITECTURE_HEADER_DIR}/pipe_config.h
  ${ARCHITECTURE_HEADER_DIR}/pipes.h)

set(ARCH_CONNECTION_HEADERS
  ${ARCHITECTURE_HEADER_DIR}/connection/socket-config.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial_status.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config_win32.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config_linux.h
  ${ARCHITECTURE_HEADER_DIR}/connection/serial-config.h
  ${ARCHITECTURE_HEADER_DIR}/connection/transport_type.h)

set(ARCH_OS_HEADERS
  ${ARCHITECTURE_HEADER_DIR}/os/linux-config/linux-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/windows-config/win32-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/mac-config/mac-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/include_defines.h
  ${ARCHITECTURE_HEADER_DIR}/os/arch.h
  ${ARCHITECTURE_HEADER_DIR}/os/os_mutex.h
  ${ARCHITECTURE_HEADER_DIR}/os/os_threads.h
  ${ARCHITECTURE_HEADER_DIR}/os/thread-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/mutex-config.h
  ${ARCHITECTURE_HEADER_DIR}/os/os_mutex.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_mutex.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_thread.h
    ${ARCHITECTURE_HEADER_DIR}/os/comm_condvar.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_semaphore.h
  ${ARCHITECTURE_HEADER_DIR}/os/comm_monitor.h)

 set(ARCHITECTURE_SOURCES
  ${ARCHITECTURE_SOURCE_DIR}/os/os_threads.cpp
  ${ARCHITECTURE_SOURCE_DIR}/os/os_mutex.cpp)

set (NGCP_GLOB
  ${NGCP_GLOB}
  ${ARCHITECTURE_HEADERS}
  ${ARCH_CONNECTION_HEADERS}
  ${ARCH_OS_HEADERS}
  ${ARCHITECTURE_SOURCES})