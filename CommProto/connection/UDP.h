#ifndef UDP_H
#define UDP_H

/* User Includes */
#include <CommProto/connection/CommsLink.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <string.h>
/** Define generic socket types */
typedef int32_t sock_fd_t;
#endif

#ifdef _WIN32
/** Define generic socket types */
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
typedef SOCKET sock_fd_t;
#endif

#define ADDRESS_LENGTH 16
class UDP: public CommsLink
{
private:
	
	struct udp_address_t
	{
		char serv[ADDRESS_LENGTH];
		uint16_t port;
	};

	struct udp_connection_t
	{		
		sock_fd_t fd;
		udp_address_t config;
		/** Keeps track of received message */
		udp_address_t last_addr;
		udp_address_t node_addr[MAX_CONNECTIONS];
		uint8_t node_connected[MAX_CONNECTIONS];
	};

	udp_connection_t conn;	

	sock_fd_t fd;	
	udp_address_t config;	
	udp_connection_t info;	
	udp_address_t rx_addr;	
	uint8_t rx_buf[MAX_BUFFER_SIZE];	
	bool connected;

	bool udp_open(sock_fd_t* fd, udp_address_t* config);

public:

	UDP(uint16_t port, char address[ADDRESS_LENGTH]);
	~UDP();
	
	bool initConnection(uint8_t port = 0, std::string address = "", uint32_t baudrate = 0);
	virtual bool addAddress(uint8_t destID, std::string address, uint16_t port);
	virtual bool removeAddress(uint8_t destID);

	virtual bool send(uint8_t pathID, uint8_t* txData, int32_t txLength);
	virtual bool recv(uint8_t* rx_data, uint32_t* rx_len);
};


#endif
