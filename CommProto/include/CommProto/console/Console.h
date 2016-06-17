#ifndef __CONSOLE_H
#define __CONSOLE_H

namespace Comnet {
namespace Console {

class Console {
public:
  Console();
  virtual ~Console();

  virtual bool setUpConnection(const char* address = "127.0.0.1");
  
  virtual bool write(const char* command = "");

  virtual char* read();
  
  virtual void display();

protected:

  virtual void update();

private:
};
}
}

#endif // __CONSOLE_H
