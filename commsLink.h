#ifndef COMMSLINK_H
#define COMMSLINK_H


#include <stdint.h>
#include <string>



#define MAX_CONNECTIONS 25

class commsLink
{
private:



public:
	commsLink();
	~commsLink();

	
	virtual bool initConnection(uint8_t port, uint32_t baudrate = 0) = 0;
	virtual bool addConnection(uint8_t destID, std::string address) = 0;
	virtual bool removeConnection(uint8_t destID) = 0;
	
	virtual bool send(uint8_t pathID, uint8_t* txData, int32_t txLength) = 0;
	virtual bool recv(uint8_t* rx_data, int32_t* rx_len) = 0;



};

#endif