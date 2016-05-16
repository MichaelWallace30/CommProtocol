#include <CommProto/comms.h>
extern "C" {
#include <architecture/os/osThreads.h>
}
/***********************************************/
/******************* Private *******************/
/***********************************************/


/**
Helper function to convert between C++ and C function signatures
due to casting as a class member being incompatible with C style
thread creation APIs. Static linkage helps with that.
*/
void* Comms::commuincation_helper(void* context)
{
	return ((Comms*)context)->commuincation_handler();
}

/** function for commuincation thread */
void* Comms::commuincation_handler()
{
	while (isRunning)
	{
		cout << "Thread Running" << endl;
		Sleep(250);//
	}

	return 0;
}


/***********************************************/
/******************* Public  *******************/
/***********************************************/
Comms::Comms()
{
	isRunning = false;	
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
}

bool Comms::initConnection(commsLink_type_t connectionType, uint8_t port, uint32_t baudrate)
{
	switch (connectionType)
	{
		case UDP_LINK:
		{}
		case SERIAL_LINK:
		{}
		case ZIGBEE_LINK:
		{}
		default:
		{return false;}
	}
	return true;
}

bool Comms::addAddress(uint8_t destID, std::string address)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->addAddress(destID, address);
}

bool Comms::removeAddress(uint8_t destID)
{
	if (connectionLayer == NULL)return false;
	return connectionLayer->removeAddress(destID);
}

bool Comms::send(uint8_t destID, uint16_t messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength)
{
	//parese headere packet here (serialize)

	if (connectionLayer == NULL) return false;
	return connectionLayer->send(messageID, buffer, messageLength);
}

bool Comms::recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint32_t &messageLength)
{
	if (connectionLayer == NULL) return false;
	bool messageFound = connectionLayer->recv(buffer, &messageLength);
	if (messageFound == true)
	{
		//need to parse header here and set source ID, messsage ID, and buffer length 
	}

	return messageFound;
}

void Comms::run()
{
	thread_create(&this->communication_thread, commuincation_helper, this);
	isRunning = true;
}

void Comms::stop()
{
	isRunning = false;
}



