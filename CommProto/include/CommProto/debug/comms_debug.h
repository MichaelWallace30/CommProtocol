/*
  Standard CommsLib debug services.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __COMMS_DEBUG_H
#define __COMMS_DEBUG_H

#include <CommProto/architecture/os/include_defines.h>
#include <stdio.h>
#include <stdarg.h>

// Controls logging information. Set this to 1, if you wish to 
// display debug information from comms_debug_log.
#if !defined(COMMPROTO_RELEASE) && (defined(_DEBUG) || defined(COMMPROTO_DEBUG))
 #define __COMMS_DEBUG_LOG 1
#else
 #define __COMMS_DEBUG_LOG 0
#endif

#if __COMMS_DEBUG_LOG
 #define __COMMS_DEBUG_PRINT
 #define comms_debug_log(debug) printf("%s\n", debug)
#else
 #define comms_debug_log(debug)
#endif // __COMMS_DEBUG

#if defined(__COMMS_DEBUG_PRINT)
 #define COMMS_DEBUG(debug, ...) printf(debug , ##__VA_ARGS__)
#else
 #define COMMS_DEBUG(debug, ...)
#endif // __COMMS_DEBUG_PRINT

#ifdef __COMMS_DEBUG_FATAL_EXIT
 #ifdef __COMMS_DEBUG_LOG
  #define comms_fatal(messg, call) { \
            comms_debug_log("FATAL: "); \
            comms_debug_log(messg); \
            call = -1; \
            exit(1); \
          }
 #else
  #define comms_fatal(messg, call) { \
            comms_debug_log("FATAL: "); \
            comms_debug_log(messg); \
            call = -1; \
          }
 #endif // __COMMS_DEBUG_FATAL_EXIT
#else
 #define comms_fatal(messg, call)
#endif // __COMMS_DEBUG_LOG


// Assertion functions are VERY useful, especially because they can tell which file the error is coming
// from, even when defined in a dll or library obj.
#define COMM_ASSERT(expression)              do { if (!(expression)) { COM_ASSERT(expression); } } while (0)
#define COMM_ASSERT_MSG(expression, message) do { if (!(expression)) { COM_ASSERT(message && expression); } } while (0)

namespace comnet {
namespace debug {



} // Console namespace
} // Comnet namespace

#endif // __COMMS_DEBUG_H 
