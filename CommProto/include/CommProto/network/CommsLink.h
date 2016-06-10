#ifndef CommsLink_H
#define CommsLink_H


#include <stdint.h>
#include <string>

#include <CommProto/CommsPacket.h>

#define MAX_CONNECTIONS 25

class CommsLink
{
public:
  CommsLink(){ ; }
  virtual ~CommsLink(){ ; }
  
  virtual bool initConnection(std::string port = "", std::string address = "", uint32_t baudrate = 0){ return false; }
  virtual bool addAddress(uint8_t destID, std::string address = "", uint16_t port = 0){ return false; }
  virtual bool removeAddress(uint8_t destID){ return false; }
  
  virtual bool send(uint8_t destID, uint8_t* txData, int32_t txLength){ return false; }
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength){ return false; }
private:
};

#endif
