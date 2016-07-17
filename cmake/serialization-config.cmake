# Tools config files, which are added into a set for compilation

set(SERIALIZATION_HEADERS
  ${SERIALIZATION_HEADER_DIR}Marshal.h
  ${SERIALIZATION_HEADER_DIR}ObjectStream.h)

set(SERIALIZATION_SOURCES
  ${SERIALIZATION_SOURCE_DIR}MarShal.cpp
  ${SERIALIZATION_SOURCE_DIR}ObjectStream.cpp)