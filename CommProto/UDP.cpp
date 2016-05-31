#include "UDP.h"

/***********************************************/
/******************* Private *******************/
/***********************************************/

bool UDP::udp_open(sock_fd_t* fd, udp_address_t* config)
{

	int32_t broadcast_enabled = 1;
	char reuse;
	struct sockaddr_in addr;
	int32_t ret;
#ifdef _WIN32
	struct WSAData* wd = (struct WSAData*)malloc(sizeof(struct WSAData));
	ret = WSAStartup(MAKEWORD(2, 0), wd);
	free(wd);
	if (ret)
	{
		return false;
	}
#endif

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(config->serv);
	addr.sin_port = htons(config->port);


	*fd = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(*fd, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast_enabled, sizeof(int32_t));
	setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	return true;
}

/***********************************************/
/******************* Public  *******************/
/***********************************************/
UDP::UDP(uint16_t port, char address[ADDRESS_LENGTH]) :CommsLink()
{
	//visual studio 2013 is having issues init array to 0
	//error is "cannot specify initializer ofr arrays knwon issues with 2013
	//make sure is node connected is set to false
	//for (int index = 0; index < MAX_CONNECTIONS; index++)
	//{
	//	conn.node_connected[index] = 0;
	//}

	memset(&info, 0, sizeof(udp_connection_t));
	memset(&config, 0, sizeof(udp_address_t));
	memset(&rx_addr, 0, sizeof(udp_address_t));
	memset(&fd, 0, sizeof(sock_fd_t));
	memset(rx_buf, 0, sizeof(MAX_BUFFER_SIZE));
	connected = false;
	config.port = port;
	strcpy(config.serv, address);
}


UDP::~UDP()
{	
}

bool UDP::initConnection(uint8_t port, std::string address, uint32_t baudrate)
{
	printf("In UDP\n");
	if (udp_open(&fd, &config))
	{		
		connected = true;
		return true;
	}

	return false;
}


bool UDP::addAddress(uint8_t destID, std::string address, uint16_t port)
{
	if (conn.node_connected[destID] == 0 && address.length() < ADDRESS_LENGTH)
	{
		
		conn.node_connected[destID] = 1;
		conn.node_addr[destID].port = port;
		address.copy(conn.node_addr[destID].serv, ADDRESS_LENGTH,0);
		return true;
	}
	
	return false;	
}

bool UDP::removeAddress(uint8_t destID)
{
	return false;
}

bool UDP::send(uint8_t pathID, uint8_t* txData, int32_t txLength)
{
	return false;
}

bool UDP::recv(uint8_t* rx_data, uint32_t* rx_len)
{
	return false;
}

