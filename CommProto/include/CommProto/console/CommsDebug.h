#ifndef __COMMS_DEBUG_H
#define __COMMS_DEBUG_H

#include <stdio.h>

// Controls logging information. Set this to 1, if you wish to 
// display debug information from comms_debug_log.
#define __COMMS_DEBUG_LOG 1

#if __COMMS_DEBUG_LOG
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
