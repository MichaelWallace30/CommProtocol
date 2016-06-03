#include <CommProto/comms.h>
#include <CommProto/architecture/os/os_mutex.h>
#include <CommProto/architecture/os/os_threads.h>

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
		
		mutex_lock(&sendMutex);
		if (!sendQeueu.empty())
		{
			
			messageStruct *tempMessage = sendQeueu.front();

			//combine header and tempMessage.dataBuff here
			uint8_t* headerBuff[HEADER_SIZE];

			memcpy(headerBuff, &tempMessage->headerBuff.destID, 1);
			memcpy(headerBuff + 1, &tempMessage->headerBuff.sourceID, 1);
			memcpy(headerBuff + 2, &tempMessage->headerBuff.messageLength, 2);
			memcpy(headerBuff + 4, &tempMessage->headerBuff.messaeID, 2);
			memcpy(headerBuff + 6, &tempMessage->headerBuff.IV, KEY_LENGTH);

			connectionLayer->send(tempMessage->headerBuff.destID, data_Buf, tempMessage->headerBuff.messageLength + HEADER_SIZE);

			delete tempMessage;
			tempMessage = NULL;
			sendQeueu.pop();
			cout << "message sent" << endl;
		}		
		mutex_unlock(&sendMutex);
		Sleep(250);//
	}

	return 0;
}

/** function for communication thread */
void* Comms::commuincationHandlerRecv()
{
	while (isRunning)
	{		
		mutex_lock(&recvMutex);
		if (connectionLayer->recv(data_Buf, &rx_length))
		{
			cout << "message recv" << endl;
			//parse data
			messageStruct *tempMessage = new messageStruct;

			memcpy((uint8_t*)tempMessage->headerBuff.destID, data_Buf, 1);
			memcpy((uint8_t*)tempMessage->headerBuff.sourceID, data_Buf + 1, 1);
			memcpy((uint8_t*)tempMessage->headerBuff.messageLength, data_Buf + 2, 2);
			memcpy((uint8_t*)tempMessage->headerBuff.messaeID, data_Buf + 4, 2);
			memcpy((uint8_t*)tempMessage->headerBuff.IV, data_Buf + 6, KEY_LENGTH);

			tempMessage->headerBuff.messageLength -= HEADER_SIZE;

			memcpy(tempMessage->dataBuff, this->data_Buf + HEADER_SIZE - 1, tempMessage->headerBuff.messageLength);

			recvQueue.push(tempMessage);
		}
		mutex_unlock(&recvMutex);
		Sleep(250);
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

bool Comms::addAddress(uint8_t destID, std::string address, uint16_t port)
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
	if (connectionLayer == NULL) return false;

	messageStruct *tempMessage = new messageStruct;
	tempMessage->headerBuff.destID = destID;
	tempMessage->headerBuff.messaeID = messageID;
	tempMessage->headerBuff.messageLength = messageLength;
	tempMessage->headerBuff.sourceID = platformID;
	memcpy(tempMessage->dataBuff, buffer, messageLength);

	mutex_lock(&sendMutex);
	sendQeueu.push(tempMessage);
	mutex_unlock(&sendMutex);
	return true;
}

bool Comms::recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint32_t &messageLength)
{
	if (connectionLayer == NULL) return false;

	mutex_lock(&recvMutex);
	bool messageFound = !recvQueue.empty();//connectionLayer->recv(buffer, &messageLength);
	if (messageFound == true)
	{
		//need to parse header here and set source ID, message ID, and buffer length 
		messageStruct *tempMessage;
		tempMessage = recvQueue.front();

		sourceID = tempMessage->headerBuff.sourceID;
		messageID = tempMessage->headerBuff.messaeID;
		messageLength = tempMessage->headerBuff.messageLength;
		memcpy(buffer, tempMessage->dataBuff, tempMessage->headerBuff.messageLength);

		delete tempMessage;
		tempMessage = NULL;		
	}
	mutex_unlock(&recvMutex);
	return messageFound;
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



