#ifndef __CONSOLE_H
#define __CONSOLE_H

namespace Comnet {
namespace Console {

/**
   Console commands for use with the debug, administration console.
*/
enum ConsoleCommands {
  CONSOLE_OPEN         = 0000,
  CONSOLE_CLOSE        = 0001,
  CONSOLE_CLEAR        = 0002,
  CONSOLE_CLEAR_SCREEN = 0003,
  CONSOLE_HOP          = 0004,
  CONSOLE_PRINT        = 0005,
  CONSOLE_TIME         = 0006,
  CONSOLE_PING         = 0007,
  CONSOLE_RESET        = 0010,
  CONSOLE_INIT         = 0011,
  CONSOLE_GOTO         = 0012,
  CONSOLE_IMPORT       = 0013,
  CONSOLE_EXPORT       = 0014,
};

/**
   Basic console interface. Intended to call Either UNIX or Windows style console. 
   this will allow user to administrate connection handling, import sheets during runtime (if possible), as 
   well as perform numerous, safe options to will offer better performance to the user's specifications.
*/
class Console {
public:
  Console();
  virtual ~Console();

  /**
     Create the console via TCP connection.
   */
  virtual bool createConsole(const char* address = "127.0.0.1");
  /**
     Write into the console. This will allow the user to send commands to the program, by means of a buffer.
   */
  virtual bool write(const char* command = "");

  /**
     Reads output from the program, back to the user for evaluation.
   */
  virtual char* read();
  
  /**
     Display, which will display information back to the user. Acts as a refresher to update the console.
   */
  virtual void display();
  /**
     Clear the screen.
   */
  virtual void clr();

protected:
  /**
     Update the console window.
   */
  virtual void update();

private:
};
}
}

#endif // __CONSOLE_H
