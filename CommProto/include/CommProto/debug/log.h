#ifndef __LOG_H
#define __LOG_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <CommProto/debug/log_status.h>

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

  virtual char* GetLogMessage() = 0;
  virtual char* Trace() = 0;

  virtual void LogMessage(const char* message) = 0;

protected:
  LogStatus log_status;
};

} // Debug namespace
} // Comnet namespace
#endif // __LOG_H
