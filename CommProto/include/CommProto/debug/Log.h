#ifndef __LOG_H
#define __LOG_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <CommProto/debug/LogStatus.h>

namespace comnet {
namespace debug {


/**
   Log interface is a notifier. It provides notifications to the Logger Manager, 
   which in turn will determine what to do with the message received.
*/
_COMNET_ABSTRACT_ class Log {
public:
  /**
     Default Constructor.
   */
  Log();
  /**
     Polymorphic destructor.
   */
  virtual ~Log();

  virtual char* getMessage() = 0;
  virtual char* trace() = 0;

  virtual void logMessage(const char* message) = 0;

protected:
  LogStatus logStatus;
};
} // Debug namespace
} // Comnet namespace
#endif // __LOG_H
