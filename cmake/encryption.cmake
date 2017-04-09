# Tools config files, which are added into a set for compilation
include_directories(SYSTEM Encryption/cryptopp563)
set(ENCRYPTION_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/include/CommProto/encryption/)
set(ENCRYPTION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CommProto/src/encryption/)


set(ENCRYPTION_HEADERS
  ${ENCRYPTION_HEADER_DIR}encryption_interface.h
  ${ENCRYPTION_HEADER_DIR}comm_random.h
  ${ENCRYPTION_HEADER_DIR}aes_encryption.h
  ${ENCRYPTION_HEADER_DIR}/decryptor.h
  ${ENCRYPTION_HEADER_DIR}/encryptor.h)
  

set(ENCRYPTION_SOURCES
${ENCRYPTION_SOURCE_DIR}aes_encryption.cpp
${ENCRYPTION_SOURCE_DIR}encryptor.cc
${ENCRYPTION_SOURCE_DIR}decryptor.cc)


set (NGCP_GLOB
  ${NGCP_GLOB}
  ${ENCRYPTION_HEADERS}
  ${ENCRYPTION_SOURCES})