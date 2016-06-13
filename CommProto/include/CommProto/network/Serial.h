#ifndef SERIAL_H
#define SERIAL_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/connection/serial.h> 
#include <CommProto/network/CommsLink.h>

#define SERIAL_DEBUG

class Serial : public CommsLink
{
private:
	serial_t hSerial;
	bool initWindows(std::string comPort, uint16_t baudrate);

	//serial private data
	bool connectionEstablished;

public:

	
	Serial();
	~Serial();

	/** Opens comport sets if it is connected on scucces, address is not need just use "" argument
	Returns false if opening comport fails*/
	virtual bool initConnection(std::string port ="", std::string address = "", uint32_t baudrate = 0);
	/** Add serial address returns true does nothing */
	virtual bool addAddress(uint8_t destID, std::string address = "", uint16_t port = 0) { return true; }
	/** Remove serial address returns true does nothing */
	virtual bool removeAddress(uint8_t destID) { return true; }
	/** Sends txData using its length of bytes through the serial connection. Connection is broadcast destID is only used for packing / unpacking
	Return false if no proper connection is establish*/	
	virtual bool send(uint8_t destID, uint8_t* txData, int32_t txLength);
	/** Sets recieved data to rxData and sets the length of the data to rxLength
	Returns false if not aviable connection, No data is recieved, or time out*/
	virtual bool recv(uint8_t* rxData, uint32_t* rxLength);
};

#endif
