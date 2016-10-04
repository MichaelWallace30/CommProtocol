# CryptoPP Library builds.

include_directories(SYSTEM Encryption/cryptopp564/include/)

set(CRYPTOPP_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Encryption/cryptopp564/include/)
set(CRYPTOPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Encryption/cryptopp564/src/)


# Header files for cryptopp.
set(CRYPTOPP_HEADERS
	${CRYPTOPP_HEADER_DIR}/aes.h
	${CRYPTOPP_HEADER_DIR}/modes.h)
	

# Source files for cryptopp.
set(CRYPTOPP_SOURCES
  ${CRYPTOPP_SOURCE_DIR}/modes.cpp)
