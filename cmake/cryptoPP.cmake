# CryptoPP Library builds.

include_directories(SYSTEM Encryption/cryptopp564/include/)

set(CRYPTOPP_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Encryption/cryptopp564/include/)
set(CRYPTOPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Encryption/cryptopp564/src/)


# Header files for cryptopp.
set(CRYPTOPP_HEADERS
	${CRYPTOPP_HEADER_DIR}/aes.h    
    ${CRYPTOPP_HEADER_DIR}/authenc.h    
    ${CRYPTOPP_HEADER_DIR}/algebra.h
    ${CRYPTOPP_HEADER_DIR}/algparam.h
    ${CRYPTOPP_HEADER_DIR}/argnames.h
    ${CRYPTOPP_HEADER_DIR}/asn.h
    ${CRYPTOPP_HEADER_DIR}/basecode.h
    ${CRYPTOPP_HEADER_DIR}/cbcmac.h
    ${CRYPTOPP_HEADER_DIR}/ccm.h       
    ${CRYPTOPP_HEADER_DIR}/channels.h
    ${CRYPTOPP_HEADER_DIR}/cmac.h  
    ${CRYPTOPP_HEADER_DIR}/config.h    
    ${CRYPTOPP_HEADER_DIR}/cpu.h    
    ${CRYPTOPP_HEADER_DIR}/cryptlib.h
    ${CRYPTOPP_HEADER_DIR}/des.h
    ${CRYPTOPP_HEADER_DIR}/dh.h
    ${CRYPTOPP_HEADER_DIR}/dll.h
    ${CRYPTOPP_HEADER_DIR}/dsa.h
    ${CRYPTOPP_HEADER_DIR}/ec2n.h
    ${CRYPTOPP_HEADER_DIR}/eccrypto.h
    ${CRYPTOPP_HEADER_DIR}/ecp.h
    ${CRYPTOPP_HEADER_DIR}/emsa2.h
    ${CRYPTOPP_HEADER_DIR}/eprecomp.h    
    ${CRYPTOPP_HEADER_DIR}/files.h
    ${CRYPTOPP_HEADER_DIR}/filters.h
    ${CRYPTOPP_HEADER_DIR}/fips140.h
    ${CRYPTOPP_HEADER_DIR}/fhmqv.h
    ${CRYPTOPP_HEADER_DIR}/fltrimpl.h
    ${CRYPTOPP_HEADER_DIR}/gcm.h
    ${CRYPTOPP_HEADER_DIR}/gf2n.h
    ${CRYPTOPP_HEADER_DIR}/gfpcrypt.h
    ${CRYPTOPP_HEADER_DIR}/hex.h
    ${CRYPTOPP_HEADER_DIR}/hmac.h
    ${CRYPTOPP_HEADER_DIR}/hmqv.h
    ${CRYPTOPP_HEADER_DIR}/hrtimer.h       
    ${CRYPTOPP_HEADER_DIR}/integer.h
    ${CRYPTOPP_HEADER_DIR}/iterhash.h    
    ${CRYPTOPP_HEADER_DIR}/misc.h
    ${CRYPTOPP_HEADER_DIR}/modarith.h
    ${CRYPTOPP_HEADER_DIR}/modes.h
    ${CRYPTOPP_HEADER_DIR}/modexppc.h    
    ${CRYPTOPP_HEADER_DIR}/mqueue.h
    ${CRYPTOPP_HEADER_DIR}/mqv.h
    ${CRYPTOPP_HEADER_DIR}/nbtheory.h
    ${CRYPTOPP_HEADER_DIR}/oaep.h
    ${CRYPTOPP_HEADER_DIR}/oids.h
    ${CRYPTOPP_HEADER_DIR}/osrng.h
    ${CRYPTOPP_HEADER_DIR}/pch.h
    ${CRYPTOPP_HEADER_DIR}/pkcspad.h
    ${CRYPTOPP_HEADER_DIR}/pssr.h
    ${CRYPTOPP_HEADER_DIR}/pubkey.h
    ${CRYPTOPP_HEADER_DIR}/queue.h
    ${CRYPTOPP_HEADER_DIR}/randpool.h
    ${CRYPTOPP_HEADER_DIR}/rijndael.h
    ${CRYPTOPP_HEADER_DIR}/rng.h
    ${CRYPTOPP_HEADER_DIR}/rsa.h
    ${CRYPTOPP_HEADER_DIR}/rw.h
    ${CRYPTOPP_HEADER_DIR}/secblock.h
    ${CRYPTOPP_HEADER_DIR}/seckey.h
    ${CRYPTOPP_HEADER_DIR}/sha.h
    ${CRYPTOPP_HEADER_DIR}/simple.h
    ${CRYPTOPP_HEADER_DIR}/skipjack.h
    ${CRYPTOPP_HEADER_DIR}/smartptr.h
    ${CRYPTOPP_HEADER_DIR}/stdcpp.h
    ${CRYPTOPP_HEADER_DIR}/strciphr.h
    ${CRYPTOPP_HEADER_DIR}/trdlocal.h
    ${CRYPTOPP_HEADER_DIR}/words.h)

# Source files for cryptopp.
set(CRYPTOPP_SOURCES
    ${CRYPTOPP_SOURCE_DIR}/algparam.cpp
    ${CRYPTOPP_SOURCE_DIR}/algebra.cpp 
    ${CRYPTOPP_SOURCE_DIR}/authenc.cpp    
    ${CRYPTOPP_SOURCE_DIR}/asn.cpp 
    ${CRYPTOPP_SOURCE_DIR}/basecode.cpp
    ${CRYPTOPP_SOURCE_DIR}/cbcmac.cpp
    ${CRYPTOPP_SOURCE_DIR}/ccm.cpp
    ${CRYPTOPP_SOURCE_DIR}/channels.cpp
    ${CRYPTOPP_SOURCE_DIR}/cmac.cpp
    ${CRYPTOPP_SOURCE_DIR}/cpu.cpp
    ${CRYPTOPP_SOURCE_DIR}/cryptlib.cpp    
    ${CRYPTOPP_SOURCE_DIR}/des.cpp
    ${CRYPTOPP_SOURCE_DIR}/dh.cpp
    ${CRYPTOPP_SOURCE_DIR}/dll.cpp
    ${CRYPTOPP_SOURCE_DIR}/dsa.cpp
    ${CRYPTOPP_SOURCE_DIR}/ec2n.cpp
    ${CRYPTOPP_SOURCE_DIR}/eccrypto.cpp
    ${CRYPTOPP_SOURCE_DIR}/ecp.cpp
    ${CRYPTOPP_SOURCE_DIR}/emsa2.cpp
    ${CRYPTOPP_SOURCE_DIR}/eprecomp.cpp
    ${CRYPTOPP_SOURCE_DIR}/files.cpp
    ${CRYPTOPP_SOURCE_DIR}/filters.cpp
    ${CRYPTOPP_SOURCE_DIR}/fips140.cpp
    ${CRYPTOPP_SOURCE_DIR}/fipstest.cpp 
    ${CRYPTOPP_SOURCE_DIR}/gcm.cpp
    ${CRYPTOPP_SOURCE_DIR}/gf2n.cpp
    ${CRYPTOPP_SOURCE_DIR}/gfpcrypt.cpp
    ${CRYPTOPP_SOURCE_DIR}/hex.cpp
    ${CRYPTOPP_SOURCE_DIR}/hmac.cpp     
    ${CRYPTOPP_SOURCE_DIR}/hrtimer.cpp 
    ${CRYPTOPP_SOURCE_DIR}/integer.cpp
    ${CRYPTOPP_SOURCE_DIR}/iterhash.cpp
    ${CRYPTOPP_SOURCE_DIR}/misc.cpp
    ${CRYPTOPP_SOURCE_DIR}/modes.cpp
    ${CRYPTOPP_SOURCE_DIR}/mqueue.cpp
    ${CRYPTOPP_SOURCE_DIR}/mqv.cpp
    ${CRYPTOPP_SOURCE_DIR}/nbtheory.cpp
    ${CRYPTOPP_SOURCE_DIR}/oaep.cpp
    ${CRYPTOPP_SOURCE_DIR}/osrng.cpp
    ${CRYPTOPP_SOURCE_DIR}/pch.cpp
    ${CRYPTOPP_SOURCE_DIR}/pkcspad.cpp
    ${CRYPTOPP_SOURCE_DIR}/pssr.cpp
    ${CRYPTOPP_SOURCE_DIR}/pubkey.cpp
    ${CRYPTOPP_SOURCE_DIR}/queue.cpp
    ${CRYPTOPP_SOURCE_DIR}/randpool.cpp
    ${CRYPTOPP_SOURCE_DIR}/rng.cpp
    ${CRYPTOPP_SOURCE_DIR}/rijndael.cpp
    ${CRYPTOPP_SOURCE_DIR}/rsa.cpp
    ${CRYPTOPP_SOURCE_DIR}/rw.cpp
    ${CRYPTOPP_SOURCE_DIR}/sha.cpp
    ${CRYPTOPP_SOURCE_DIR}/simple.cpp
    ${CRYPTOPP_SOURCE_DIR}/skipjack.cpp
    ${CRYPTOPP_SOURCE_DIR}/strciphr.cpp
    ${CRYPTOPP_SOURCE_DIR}/trdlocal.cpp)


# Glob the files.
set (NGCP_GLOB
  ${NGCP_GLOB}
  ${CRYPTOPP_HEADERS}
  ${CRYPTOPP_SOURCES}) 