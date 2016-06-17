#include <CommProto/network/UDP.h>

/***********************************************/
/******************* Private *******************/
/***********************************************/


bool UDP::udp_open(int* fd)
{
  bool result = false;
  initializeWSAStartup(result);
  /** attempts to open socket 
      returns false if fails*/
  if ((*fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
      printf("socket() failed\n");
      result = false;;
  }
  
  return result;
}

void UDP::stringToChar(char cPtr[ADDRESS_LENGTH], std::string str)
{
  memset(cPtr, 0, ADDRESS_LENGTH);
  int maxChar;
  (str.length() > ADDRESS_LENGTH) ? maxChar = ADDRESS_LENGTH : maxChar = str.length();
  for (int index = 0; index < maxChar; index++)
    {
      cPtr[index] = str[index];
    }
}


/***********************************************/
/******************* Public  *******************/
/***********************************************/
UDP::UDP() : CommsLink()
{
  //visual studio 2013 is having issues init array to 0
  //error is "cannot specify initializer of arrays knwon issues with 2013
  //make sure is node connected is set to false
  for (int index = 0; index < MAX_CONNECTIONS; index++)
    {
      conn[index].socket_status = SOCKET_OPEN;
    }
  
//  connected = false;
  sockaddr.socket_status = SOCKET_OPEN;
}


UDP::~UDP()
{	
  closeSocket(fd);
}

bool UDP::initConnection(std::string port, std::string address, uint32_t baudrate)
{	
  //open socket and  check if socket is not already connected
  if (sockaddr.socket_status != SOCKET_CONNECTED && udp_open(&fd))
    {


	  ///check if port in number
		for (int x = 0; x < port.length(); x++)
		{

			if (!isdigit(port[x]))
			{
				printf("initConnection 'port' argument is not a numerical digit for udp connection\n");
				return false;
			}
		}
	  uint32_t portInt = atoi(port.c_str());


      char tempChar[ADDRESS_LENGTH];
      stringToChar(tempChar, address);
      
      //setup address structure
      memset((char *)&sockaddr.socket_address, 0, sizeof(sockaddr.socket_address));
      sockaddr.socket_address.sin_family = AF_INET;
      sockaddr.socket_address.sin_port = htons(portInt);
      sockaddr.socket_address.sin_addr.s_addr = inet_addr(tempChar);
      
      //bind socket
      if (bind(fd, (struct sockaddr *)&sockaddr.socket_address, sizeof(sockaddr.socket_address)) < 0) {
	printf("bind failed");
	return false;
      }
      
      //set size of address
      slen = sizeof(sockaddr.socket_address);
      //set connected to true
      sockaddr.socket_status = SOCKET_CONNECTED;
      return true;
    }
  //already connected or failed to open socket
  return false;
}


bool UDP::addAddress(uint8_t destID, std::string address, uint16_t port)
{
  if (conn[destID].socket_status == SOCKET_OPEN && address.length() < ADDRESS_LENGTH)
    {
      //convert string to char*
      char tempChar[ADDRESS_LENGTH];
      stringToChar(tempChar, address);
      
      //setup address structure
      memset((char *)&conn[destID].socket_address, 0, sizeof(conn[destID].socket_address));
      conn[destID].socket_address.sin_family = AF_INET;
      conn[destID].socket_address.sin_port = htons(port);
      conn[destID].socket_address.sin_addr.s_addr = inet_addr(tempChar);
      conn[destID].socket_status = SOCKET_CONNECTED;
      return true;
    }
  
  //already connected node or address is invalid
  return false;	
}

bool UDP::removeAddress(uint8_t destID)
{
  
  if (conn[destID].socket_status == SOCKET_CONNECTED)
    {
      conn[destID].socket_status = SOCKET_OPEN;
      return true;
    }
  return false;
}

bool UDP::send(uint8_t destID, uint8_t* txData, int32_t txLength)
{
  if (sockaddr.socket_status == SOCKET_CONNECTED)
    {
      int slenSend = sizeof(conn[destID].socket_address);
      if (sendto(fd, 
		 (char*)txData, 
		 txLength, 0, 
		 (struct sockaddr *) 
		 &conn[destID].socket_address, slen) < 0)
		{
			printf("sendto() failed\n");
			return false;
		}
		else
		{	  
		#ifdef UDP_DEBUG
			int port = ntohs(conn[destID].socket_address.sin_port);				
			printf("**  Sent\t Length: %d, Port: %d, IP: %s **\n", 
				txLength, port, 
				inet_ntoa(conn[destID].socket_address.sin_addr));
		#endif	  
		}
    }
  return false;
}

bool UDP::recv(uint8_t* rxData, uint32_t* rxLength)
{
  int length = 0;
  *rxLength = 0;
  if (sockaddr.socket_status == SOCKET_CONNECTED)
    {
      
      length = recvfrom(fd, 
			(char*)rxData, MAX_BUFFER_SIZE, 
			0, 
			(struct sockaddr *) &si_other.socket_address, 
			(socklen_t*)&slen);
			}
			else
			{
				printf("UDP not connected can't receive\n");
				return false;//not connected
			}
  
  if (length < 0) return false;
  
#ifdef UDP_DEBUG
  int port = ntohs(si_other.socket_address.sin_port);
  printf("**  Recieved\t Length: %d, Port: %d, IP: %s **\n", length, port, inet_ntoa(si_other.socket_address.sin_addr));
#endif
  
  *rxLength = (uint32_t)length;
  return true;
}

