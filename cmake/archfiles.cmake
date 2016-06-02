# Architecture files defined and configured in this .cmake file.

set(ARCHITECTURE_HEADERS 
  architecture/os/linux-config/linux-config.h
  architecture/os/include_defines.h
  architecture/os/arch.h
  architecture/os/os_threads.h
  architecture/os/thread-config.h
  architecture/os/mutex-config.h
  architecture/os/os_mutex.h
  architecture/api.h
  architecture/macros.h)

set(ARCHITECTURE_SOURCES
  architecture/os/os_threads.c
  architecture/os/os_mutex.c)
