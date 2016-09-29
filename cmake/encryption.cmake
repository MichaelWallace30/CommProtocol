# Tools config files, which are added into a set for compilation
set(ENCRYPTION_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/encryption/)
set(ENCRYPTION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/encryption/)


set(ENCRYPTION_HEADERS
  ${ENCRYPTION_HEADER_DIR}encryptionInterface.h
  ${ENCRYPTION_HEADER_DIR}aesEncryption.h)
  

set(ENCRYPTION_SOURCES
${ENCRYPTION_SOURCE_DIR}aesEncryption.cpp)