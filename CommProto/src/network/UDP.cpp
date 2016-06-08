#include <CommProto/network/UDP.h>

/***********************************************/
/******************* Private *******************/
/***********************************************/

bool UDP::udp_open(int* fd)
{
#ifdef WIN32//windows bs
  WSADATA wsa;
  //Initialise winsock
  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
      printf("Failed. Error Code : %d", WSAGetLastError());
      return 0;
    }
  printf("Initialised.\n");
#endif

	
  /** attempts to open socket 
      returns false if fails*/
  if ((*fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("socket() failed\n");
      return false;
    }
  
  
  return true;
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
      conn[index].node_connected = false;
    }
  
  connected = false;
  
}


UDP::~UDP()
{	
  closSocket(fd);
}

bool UDP::initConnection(uint16_t port, std::string address, uint32_t baudrate)
{	
  //open socket and  check if socket is not already connected
  if (!connected && udp_open(&fd))
    {	
      char tempChar[ADDRESS_LENGTH];
      stringToChar(tempChar, address);
      
      //setup address structure
      memset((char *)&sockaddr, 0, sizeof(sockaddr));
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_port = htons(port);
      sockaddr.sin_addr.s_addr = inet_addr(tempChar);
      
      //bind socket
      if (bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
	printf("bind failed");
	return false;
      }
      
      //set size of address
      slen = sizeof(sockaddr);
      //set connected to true
      connected = true;
      return true;
    }
  //already connected or failed to open socket
  return false;
}


bool UDP::addAddress(uint8_t destID, std::string address, uint16_t port)
{
  if (conn[destID].node_connected == 0 && address.length() < ADDRESS_LENGTH)
    {
      //convert string to char*
      char tempChar[ADDRESS_LENGTH];
      stringToChar(tempChar, address);
      
      //setup address structure
      memset((char *)&conn[destID].sockaddr, 0, sizeof(conn[destID].sockaddr));
      conn[destID].sockaddr.sin_family = AF_INET;
      conn[destID].sockaddr.sin_port = htons(port);
      conn[destID].sockaddr.sin_addr.s_addr = inet_addr(tempChar);
      conn[destID].node_connected = true;
      return true;
    }
  
  //already connected node or address is invalid
  return false;	
}

bool UDP::removeAddress(uint8_t destID)
{
  
  if (conn[destID].node_connected)
    {
      conn[destID].node_connected = false;
      return true;
    }
  return false;
}

bool UDP::send(uint8_t destID, uint8_t* txData, int32_t txLength)
{
  if (connected)
    {
      int slenSend = sizeof(conn[destID].sockaddr);
      if (sendto(fd, 
		 (char*)txData, 
		 txLength, 0, 
		 (struct sockaddr *) 
		 &conn[destID].sockaddr, slen) < 0)
	{
	  printf("sendto() failed\n");
	  return false;
	}
      else
	{
	  
#ifdef UDP_DEBUG
	  int port = ntohs(conn[destID].sockaddr.sin_port);				
	  printf("**  Sent\t Length: %d, Port: %d, IP: %s **\n", 
		 txLength, port, 
		 inet_ntoa(conn[destID].sockaddr.sin_addr));
#endif
	  
	}
    }
  return false;
}

bool UDP::recv(uint8_t* rx_data, uint32_t* rx_len)
{
  int length = 0;
  *rx_len = 0;
  if (connected)
    {
      
      length = recvfrom(fd, 
			(char*)rx_data, MAX_BUFFER_SIZE, 
			0, 
			(struct sockaddr *) &si_other, 
			(socklen_t*)&slen);
    }
  else
    {
      printf("UDP not connected can't receive\n");
      return false;//not connected
    }
  
  if (length < 0) return false;
  
#ifdef UDP_DEBUG
  int port = ntohs(si_other.sin_port);
  printf("**  Recieved\t Length: %d, Port: %d, IP: %s **\n", length, port, inet_ntoa(si_other.sin_addr));
#endif
  
  *rx_len = (uint32_t)length;
  return true;
}

