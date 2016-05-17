#ifndef COMMSLINK_H
#define COMMSLINK_H


#include <stdint.h>
#include <string>

#define MAX_CONNECTIONS 25

class commsLink
{
private:



public:
	commsLink(){};
	~commsLink(){};
	
	virtual bool initConnection(uint8_t port, uint32_t baudrate = 0){ return false; }
	virtual bool addAddress(uint8_t destID, std::string address){ return false; }
	virtual bool removeAddress(uint8_t destID){ return false; }
	
	virtual bool send(uint8_t pathID, uint8_t* txData, int32_t txLength){ return false; }
	virtual bool recv(uint8_t* rx_data, uint32_t* rx_len){ return false; }
};

#endif