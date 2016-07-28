#include <CommProto/Comms.h>

#include <CommProto/network/UDP.h>
#include <CommProto/network/Serial.h>

using namespace Comnet;

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
		if (getID() == 1)
		{
			uint8_t buff[10] = { 1 };
			connectionLayer->send(2, buff, 10);
			Sleep(1500);
		}

	}

	return 0;
}

/** function for communication thread */
void* Comms::commuincationHandlerRecv()
{
	while (isRunning)
	{		
		if (getID() == 2)
		{

			connectionLayer->recv(data_Buf, &rx_length);
			Sleep(1500);
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

bool Comms::initConnection(CommsLink_type_t connectionType, const char* port, const char* address, uint32_t baudrate)
{
	uint16_t length = 0;
	switch (connectionType)
	{
		case UDP_LINK:
		{			
			
			str_length(address, length);
			if (length < ADDRESS_LENGTH)
			{							
				connectionLayer = new UDP();
				return connectionLayer->initConnection(port, address);
			}
			break;
		}
		case SERIAL_LINK:
		{
			
			str_length(address, length);
			if (length < ADDRESS_LENGTH)
			{
				connectionLayer = new Serial();
				return connectionLayer->initConnection(port, NULL, baudrate);
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

bool Comms::addAddress(uint8_t destID, const char* address , uint16_t port)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->addAddress(destID, address, port);
}

bool Comms::removeAddress(uint8_t destID)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->removeAddress(destID);
}

bool Comms::send(AbstractPacket* packet, uint8_t destId, uint16_t messageId)
{
	if (connectionLayer == NULL) return false;

	return true;
}

AbstractPacket* Comms::receive(uint8_t&  sourceId, uint16_t& messageId)
{
	if (connectionLayer == NULL) return false;
	
	

	return NULL;
}

int32_t Comms::run()
{
	thread_create(&this->communicationThreadSend, commuincationHelperSend, this);
	thread_create(&this->communicationThreadRecv, commuincationHelperRecv, this);
	isRunning = true;

	return 1;
}

int32_t Comms::pause()
{
	return 0;
}

int32_t Comms::stop()
{
	isRunning = false;
	return 1;
}



