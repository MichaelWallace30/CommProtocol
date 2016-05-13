#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>//needed for bit size variables
#include <queue>
#include <string>//needed for addresses

#include "commsPacket.h"//Header which declares packet structs

#include "architecture\os\include_defines.h"
#include "architecture\os\osThreads.h"//method to create threads
#include "commsLink.h"//commincation layer interface/abstract base class


#define MAX_BUFFER_SIZE MAX_PACKET_SIZE + HEADER_SIZE


#include <iostream>//testing only
using namespace std;//testing only

typedef enum
{
	UDP_LINK,
	SERIAL_LINK,
	ZIGBEE_LINK
}commsLink_type_t;

class Comms{
   

private:
	/** Boolean if communication thread running*/
	bool isRunning;
    /** Encryotion key*/
	uint8_t key[KEY_LENGTH];
    /** Method to read key form text file*/
    void loadKey();

	/** Data buffer to create communication stream with */
	uint8_t data_Buf[MAX_BUFFER_SIZE];

	/** Platform ID */
	uint8_t platformID;
        
	/** Queues for application layer to push messages or pop messages */
	std::queue<uint8_t*> recvQueue;
	std::queue<uint8_t*> sendQeueu;

	/** Array of connections to match destination ID to address string */
	std::string connections[MAX_CONNECTIONS];
   
   /** Thread to run commuincation data */
   thread_t communication_thread;
   
   /** Method to run in commuincation thread */
   void* commuincation_handler();
   
   /**
   Helper function to convert between C++ and C function signatures
   due to casting as a class member being incompatible with C style
   thread creation APIs. Static linkage helps with that.
   */
   static void* commuincation_helper(void* context);  

public:
    Comms();
    /** Constuctor */
    Comms(uint8_t platformID);
    /** Destcutor */
    ~Comms();
    
    /** platform ID mutator */
    void setID(uint8_t platformID){this->platformID = platformID;}
    /** platform ID accessor */
    uint8_t getID(){return platformID;}
    
	/** Method will init the connection device and return false if connection failed to init properly 
	enum connection type to use serial, udp, zigbee, ect
	port number is the comport or udp port used will differ from windwos and unix for comports COM05 or /dev/ttyUSB05
	baudrate is not used for for udp which is not needed but for serial and zigbee it is needed for baud rate */	
	bool initConnection(commsLink_type_t connectionType, uint8_t port, uint32_t baudrate = 0);

	/** The address entered will be paired for commuincation by destination ID
	Adding address can be a udp IPV4 or hex MAC address for zigbee
	Adding an adress is not need for serial and will default to ""*/
	bool addAddress(uint8_t destID, std::string address = "");

	/** Removing an address removes the known assocciation of address and destination ID by using id*/
	bool removeAddress(uint8_t destID);
    
	bool send(uint8_t destID, uint16_t messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	bool recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	
	/** Method to start communication*/
	void run();
	/** Method to stop communication*/
	void stop();



    
};//end Comms class
#endif//end if COMMS_H
