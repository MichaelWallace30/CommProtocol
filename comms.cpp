#include "comms.h"



/******************* Private *******************/


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
		Sleep(200);//
	}

	return 0;
}

/******************* Public  *******************/
Comms::Comms()
{
	isRunning = false;
}

Comms::Comms(uint8_t platformID)
{
	Comms();
	setID(platformID);
}

Comms::~Comms()
{
	isRunning = false;
	Sleep(300);//change to proper sleep
}

bool initConnection(uint8_t port, uint32_t baudrate = 0)
{
	return true;
}

bool addConnection(uint8_t destID, std::string address)
{
	return true;
}

bool removeConnection(uint8_t destID)
{
	return true;
}

bool send(uint8_t destID, char messageID[MESSAGE_ID_SIZE], uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength)
{
	return true;
}

bool recv(uint8_t &sourceID, char messageID[MESSAGE_ID_SIZE], uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength)
{
	return true;
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



