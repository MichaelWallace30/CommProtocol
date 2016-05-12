#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include <queue>
#include <string>

#include "commsPacket.h"//Header which declares packet structs

#include "architecture\os\include_defines.h"
#include "osThreads.h"
#include "commsLink.h"


#define MAX_BUFFER_SIZE MAX_PACKET_SIZE + HEADER_SIZE


#include <iostream>
using namespace std;



typedef enum
{
	UDP_LINK,
	SERIAL_LINK,
	ZIGBEE_LINK
}commsLink_type_t;

class Comms{
   

private:
	/** Boolean if thread is running*/
	bool isRunning;
    /** Add key to comnet class*/
	uint8_t key[KEY_LENGTH];
    /** Method to read key form text file*/
    void loadKey();

	/** Data buffer to create packets with */
	uint8_t data_Buf[MAX_BUFFER_SIZE];

	/** Platform ID */
	uint8_t platformID;
        
	/** Queues for application layer to push message or pop messages */
	std::queue<uint8_t*> recvQueue;
	std::queue<uint8_t*> sendQeueu;

	/** Array of connections to match destion ID to address string */
	std::string connections[MAX_CONNECTIONS];
   
   /** Thread to run commuincation data */
   thread_t communication_thread;
   
   /** function to fun in commuincation thread */
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
    
    /** platform mutator */
    void setID(uint8_t platformID){this->platformID = platformID;}
    /** platform accessor */
    uint8_t getID(){return platformID;}
    
	//I love bool
	bool initConnection(commsLink_type_t connectionType, uint8_t port, uint32_t baudrate = 0);
	bool addAddress(uint8_t destID, std::string address);
	bool removeAddress(uint8_t destID);
    
	bool send(uint8_t destID, uint16_t messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	bool recv(uint8_t &sourceID, uint16_t &messageID, uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	
	void run();
	void stop();



    
};//end Comms class
#endif//end if COMMS_H
