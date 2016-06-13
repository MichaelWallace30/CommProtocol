#include <CommProto/comms.h>

#include <CommProto/network/UDP.h>
#include <CommProto/network/Serial.h>
/***********************************************/
/******************* Private *******************/
/***********************************************/


/**
Helper function to convert between C++ and C function signatures
due to casting as a class member being incompatible with C style
thread creation APIs. Static linkage helps with that.
*/
void* Comms::commuincationHelperSend(void* context)
{
	return ((Comms*)context)->commuincationHandlerSend();
}

void* Comms::commuincationHelperRecv(void* context)
{
	return ((Comms*)context)->commuincationHandlerRecv();
}


/** function for communication thread */
void* Comms::commuincationHandlerSend()
{
	while (isRunning)
	{
		if (platformID == 1)
		{
			uint8_t buff[10] = { 1 };
			connectionLayer->send(2, buff, 10);
			Sleep(250);
			
		}

	}

	return 0;
}

/** function for communication thread */
void* Comms::commuincationHandlerRecv()
{
	while (isRunning)
	{		
		if (platformID == 2)
		{

			connectionLayer->recv(data_Buf, &rx_length);
			if (rx_length > 0)
			{
				
			}
		}

	}

	return 0;
}

/***********************************************/
/******************* Public  *******************/
/***********************************************/
Comms::Comms()
{
	isRunning = false;	
	mutex_init(&sendMutex);
	mutex_init(&recvMutex);	
	connectionLayer = NULL;
}

Comms::Comms(uint8_t platformID)
{
	Comms();	
	setID(platformID);
}

Comms::~Comms()
{
	isRunning = false;
	if(connectionLayer != NULL)
	{
		delete connectionLayer;
	}
	mutex_destroy(&sendMutex);
	mutex_destroy(&recvMutex);
}

bool Comms::initConnection(CommsLink_type_t connectionType, std::string port, std::string address, uint32_t baudrate)
{
	switch (connectionType)
	{
		case UDP_LINK:
		{			
			if (address.length() < ADDRESS_LENGTH)
			{							
				connectionLayer = new UDP();
				return connectionLayer->initConnection(port, address);
			}
			break;
		}
		case SERIAL_LINK:
		{
			if (address.length() < ADDRESS_LENGTH)
			{
				connectionLayer = new Serial();
				return connectionLayer->initConnection(port, "", baudrate);
			}
			break;
		
		}
		case ZIGBEE_LINK:
		{}
		default:
		{return false;}
	}
	return true;
}

bool Comms::addAddress(uint8_t destID, std::string address, uint16_t port)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->addAddress(destID, address, port);
}

bool Comms::removeAddress(uint8_t destID)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->removeAddress(destID);
}

bool Comms::send(uint8_t destID, uint16_t messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength)
{
	if (connectionLayer == NULL) return false;

	return true;
}

bool Comms::recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint32_t &messageLength)
{
	if (connectionLayer == NULL) return false;
	
	

	return true;
}

void Comms::run()
{
	thread_create(&this->communicationThreadSend, commuincationHelperSend, this);
	thread_create(&this->communicationThreadRecv, commuincationHelperRecv, this);
	isRunning = true;
}

void Comms::stop()
{
	isRunning = false;
}



