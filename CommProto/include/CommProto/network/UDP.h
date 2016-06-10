#ifndef UDP_H
#define UDP_H

/* User Includes */
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/network/CommsLink.h>
#include <CommProto/architecture/connection/socket-config.h>

#define UDP_DEBUG
#define ADDRESS_LENGTH 16

class UDP: public CommsLink
{
private:
  
  
  /**Struct
     -sockaddr_in: IP address & port
     -node_connected: if node is conneted than true*/
  struct udp_connection_t
  {		
    struct sockaddr_in sockaddr;			
    bool node_connected;
  };
  
  /** Array of connections: address, port, and if connceted*/
  udp_connection_t conn[MAX_CONNECTIONS];
  
  /** Socket id (socket descriptor returned by socket)*/
  int fd;	
  /** Local address & port */
  struct sockaddr_in sockaddr;
  /** recieved address & port */
  struct sockaddr_in si_other;
  /** Local address length */
  int slen;
  
  /** message buffer */
  uint8_t rx_buf[MAX_BUFFER_SIZE];	
  
  /** Is local socket connected */
  bool connected;
  
  /** Opens udp socket returns false if socket open fails*/
  bool udp_open(int* fd);
  
  /** Deep copy of string to char array */
  void stringToChar(char cPtr[ADDRESS_LENGTH], std::string str);
  
  
 public:
  
  /**Constuctor*/
  UDP();
  ~UDP();
  
  /** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
      Returns false if open socket or bind fails*/
  bool initConnection(std::string port = "", std::string address = "", uint32_t baudrate = 0);
  /** Adds Address & port to destID value of array of aviable connections
      Returns false if connection is already connected*/
  virtual bool addAddress(uint8_t destID, std::string address ="", uint16_t port = 0);
  /** Sets connection to not available
      Returns false is no connection is found*/
  virtual bool removeAddress(uint8_t destID);
  /** Sends txData using its length of bytes through the destID connection which is establish through add adress
      Return false if no proper connection is establish*/
  virtual bool send(uint8_t destID, uint8_t* txData, int32_t txLength);
  /** Sets recieved data to rxData and sets the length of the data to rxLength
      Returns false if not aviable connection or no data is recieved*/
  virtual bool recv(uint8_t* rxData, uint32_t* rxLength);
  
};


#endif
