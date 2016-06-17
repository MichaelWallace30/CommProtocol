#ifndef __COMMS_DEBUG_H
#define __COMMS_DEBUG_H

#include <stdio.h>

#ifdef __COMMS_DEBUG__LOG
 #define comms_debug_log(debug) printf("%s\n", debug);
#else
 #define comms_debug_log(debug)
#endif // __COMMS_DEBUG


#ifdef __COMMS_DEBUG_FATAL_EXIT
 #ifdef __COMMS_DEBUG_LOG
  #define comms_fatal(messg, call) { \
            comms_debug_log("FATAL: "); \
            comms_debug_log("%s", messg); \
	    call = -1; \
          }
 #else
  #define comms_fatal(messg, call)
 #endif // __COMMS_DEBUG_FATAL_EXIT
#else
 #define comms_fatal(messg, call)
#endif // __COMMS_DEBUG_LOG

namespace Comnet {
namespace Console {



} // Console namespace
} // Comnet namespace

#endif // __COMMS_DEBUG_H 
