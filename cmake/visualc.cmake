# visual studio compilation

# Checking visual studio build.
if (${CXX_COMPILER} STREQUAL "MSVC") 
  message("Visual Studio build")
endif()

# Building the header and source directory.
if (${CMAKE_CXX_COMPILER_ID})    
    source_group("Header Files\\architecture" FILES ${ARCHITECTURE_HEADERS})
    source_group("Header Files\\hash" FILES ${COMMPROTO_HASH_HEADERS})
    source_group("Header Files\\tools" FILES ${TOOLS_HEADERS})
    source_group("Header Files\\network" FILES ${NETWORK_HEADERS})
    source_group("Header Files\\serialization" FILES ${SERIALIZATION_HEADERS})
    source_group("Header Files\\pkg" FILES ${COMMPROTO_PKG_HEADERS})
    source_group("Header Files\\debug" FILES ${DEBUG_HEADERS})
    source_group("Header Files\\encryption" FILES ${ENCRYPTION_HEADERS})
    source_group("Header Files\\xbee_ansic" FILES ${XBEE_HEADERS})
	source_group("Header Files\\encryption\\cryptoPP" FILES ${CRYPTOPP_HEADERS})
    source_group("Source Files\\architecture" FILES ${ARCHITECTURE_SOURCES})
    source_group("Source Files\\hash" FILES ${COMMPROTO_HASH_SOURCES})
    source_group("Source Files\\network" FILES ${NETWORK_SOURCES})
    source_group("Source Files\\serialization" FILES ${SERIALIZATION_SOURCES})
    source_group("Source Files\\pkg" FILES ${COMMPROTO_PKG_SOURCES})
    source_group("Source Files\\encryption" FILES ${ENCRYPTION_SOURCES})
    source_group("Source Files\\xbee_ansic" FILES ${XBEE_SOURCES})
	source_group("Source Files\\encryption\\cryptoPP" FILES ${CRYPTOPP_SOURCES})
endif()