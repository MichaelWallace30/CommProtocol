#include <CommProto/comms.h>
#include <CommProto/architecture/os/os_threads.c>

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

/** function for communication thread */
void* Comms::commuincation_handler()
{
	while (isRunning)
	{
		cout << "Thread Running" << endl;
		if (!sendQeueu.empty())
		{
			uint8_t *tempPtr = sendQeueu.front();
			//change to marshal this is bs way
			uint16_t *tx_length;
			memcpy(tx_length, (uint8_t*)&tempPtr[2], 2);

			connectionLayer->send(tempPtr[0], tempPtr, *tx_length);
		}
		
		
		if (connectionLayer->recv(data_Buf, &rx_length))
		{
			//parse data
			uint8_t* newBuff = new uint8_t[rx_length];
			memcpy(newBuff, data_Buf, rx_length);
			recvQueue.push(newBuff);
		}
		
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

bool Comms::initConnection(CommsLink_type_t connectionType, uint8_t port, std::string address, uint32_t baudrate)
{
	switch (connectionType)
	{
		case UDP_LINK:
		{
			char addr[ADDRESS_LENGTH];
			if (address.length() < ADDRESS_LENGTH)
			{
				address.copy(addr, ADDRESS_LENGTH, 0);				
				connectionLayer = new UDP(port, addr);
				return connectionLayer->initConnection();
			}
			break;
		}
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
	//parse header packet here (serialize)
	

	if (connectionLayer == NULL) return false;
	sendQeueu.push(buffer);
	return true;
}

bool Comms::recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint32_t &messageLength)
{
	if (connectionLayer == NULL) return false;
	bool messageFound = !recvQueue.empty();//connectionLayer->recv(buffer, &messageLength);
	if (messageFound == true)
	{
		//need to parse header here and set source ID, message ID, and buffer length 
		
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



