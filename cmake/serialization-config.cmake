# Tools config files, which are added into a set for compilation
set(SERIALIZATION_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/serialization/)
set(SERIALIZATION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/serialization/)

set(SERIALIZATION_HEADERS
  ${SERIALIZATION_HEADER_DIR}Marshal.h
  ${SERIALIZATION_HEADER_DIR}ObjectStream.h)

set(SERIALIZATION_SOURCES
  ${SERIALIZATION_SOURCE_DIR}Marshal.cpp
  ${SERIALIZATION_SOURCE_DIR}ObjectStream.cpp)