#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>//needed for bit size variables
#include <queue>
#include <string>//needed for addresses

#include <CommProto/network/CommsLink.h> //communication layer interface/abstract base class

#include <CommProto/network/TcpSocket.h> // 

#include <CommProto/CommsPacket.h>//Header which declares packet structs
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/os/os_threads.h>//method to create threads
#include <CommProto/architecture/os/os_mutex.h>


#include <iostream>//testing only
using namespace std;//testing only

typedef enum
{
	UDP_LINK,
	SERIAL_LINK,
	ZIGBEE_LINK
}CommsLink_type_t;

class Comms {
private:
  mutex_t sendMutex;
  mutex_t recvMutex;

	/** Boolean if communication thread running*/
	bool isRunning;
    /** Encryption key*/
	uint8_t key[KEY_LENGTH];
    /** Method to read key form text file*/
    void loadKey();

	/** Data buffer to create communication stream with */
	uint8_t data_Buf[MAX_BUFFER_SIZE];
	/** Length of data buffer for communication stream */
	uint32_t rx_length;

	/** Platform ID */
	uint8_t platformID;
        
	struct messageStruct
	{
		uint8_t dataBuff[MAX_PACKET_SIZE];
		header_t headerBuff;
	};

	/** Queues for application layer to push messages or pop messages */
	std::queue<messageStruct*> recvQueue;
	std::queue<messageStruct*> sendQeueu;
  
   /** Thread to run communication data */
   thread_t communicationThreadSend;
   thread_t communicationThreadRecv;
   
   /** Method to run in communication thread */
   void* commuincationHandlerSend();
   void* commuincationHandlerRecv();
   
   /**
   Helper function to convert between C++ and C function signatures
   due to casting as a class member being incompatible with C style
   thread creation APIs. Static linkage helps with that.
   */
   static void* commuincationHelperSend(void* context);  
   static void* commuincationHelperRecv(void* context);

   /** Virtual Communication link for connection code*/
   CommsLink *connectionLayer;


   /** Parse header function de-serialize header packet */
   void deserializeHeader();
   
   /** Parse header function to serialize header packet */
   void serializeHeader();
public:
    Comms();
    /** Constructor */
    Comms(uint8_t platformID);
    /** Destructor */
    ~Comms();
    
    /** platform ID mutator */
    void setID(uint8_t platformID)
      {this->platformID = platformID;}
    /** platform ID accessors */
    uint8_t getID() 
      {return platformID;}
    
	/** Method will init the connection device and return false if connection failed to init properly 
	enum connection type to use serial, UDP, zigbee, ect
	port number is the comport or UDP port used will differ from windows and Unix for comports COM05 or /dev/ttyUSB05
	baud-rate is not used for for UDP which is not needed but for serial and zigbee it is needed for baud rate */	
	bool initConnection(CommsLink_type_t connectionType, std::string port, std::string address = "", uint32_t baudrate = 0);

	/** The address entered will be paired for communication by destination ID
	Adding address can be a UDP IPV4 or hex MAC address for zigbee
	Adding an address is not need for serial and will default to ""*/
	bool addAddress(uint8_t destID, std::string address="", uint16_t port = 0);

	/** Removing an address removes the known association of address and destination ID by using id*/
	bool removeAddress(uint8_t destID);
    
	bool send(uint8_t destID, uint16_t messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	bool recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint32_t &messageLength);
	
	/** Method to start communication*/
	void run();
	/** Method to stop communication*/
	void stop();

protected:
    // Nothing yet.
    void logToConsoles() 
      { }
};//end Comms class
#endif//end if COMMS_H
