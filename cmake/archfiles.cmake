# Architecture files defined and configured in this .cmake file.

set(ARCHITECTURE_HEADERS
  ${PARENT_HEADER_DIR}/architecture/os/linux-config/linux-config.h
  ${PARENT_HEADER_DIR}/architecture/os/include_defines.h
  ${PARENT_HEADER_DIR}/architecture/os/arch.h
  ${PARENT_HEADER_DIR}/architecture/os/os_threads.h
  ${PARENT_HEADER_DIR}/architecture/os/thread-config.h
  ${PARENT_HEADER_DIR}/architecture/os/mutex-config.h
  ${PARENT_HEADER_DIR}/architecture/os/os_mutex.h
  ${PARENT_HEADER_DIR}/architecture/api.h
  ${PARENT_HEADER_DIR}/architecture/macros.h
  ${PARENT_HEADER_DIR}/architecture/connection/socket-config.h
  ${PARENT_HEADER_DIR}/architecture/connection/serial_status.h
  ${PARENT_HEADER_DIR}/architecture/connection/serial-config_win32.h
  ${PARENT_HEADER_DIR}/architecture/connection/serial-config_linux.h
  ${PARENT_HEADER_DIR}/architecture/connection/serial-config.h
  ${PARENT_HEADER_DIR}/architecture/connection/transport_type.h)

set(ARCHITECTURE_SOURCES
  ${PARENT_SOURCE_DIR}/architecture/os/os_threads.cpp
  ${PARENT_SOURCE_DIR}/architecture/os/os_mutex.cpp)
