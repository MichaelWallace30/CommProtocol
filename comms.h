#include <stdint.h>
#include <queue>
#include <string>

#include "CommsPacket.h"//Header which declares packet structs

#define MAX_CONNECTIONS 25


#ifndef COMMS_H
#define COMMS_H

class Comms{
   

private:
    /** Add key to comnet class*/
	uint8_t key[KEY_LENGTH];
    /** Method to read key form text file*/
    void loadKey();

	/** Platform ID */
	uint8_t plarformID;
        
	/** Queues for application layer to push message or pop messages */
	std::queue<uint8_t*> recvQueue;
	std::queue<uint8_t*> sendQeueu;

	/** Array of connections to match destion ID to address string */
	std::string connections[MAX_CONNECTIONS];
   
   //thread method
        //recv: recv, build packet, decrypt, and queue  
        //send: deque , encrypt, and send
   
    
    
public:
    /** Constuctor */
    Comms(uint8_t platformID);
    /** Destcutor */
    ~Comms();
    
	//I love bool
	//maybe enum paramter for connection type for init method?
	bool initConnection(uint8_t port, uint32_t baudrate = 0);
	bool addConnection(uint8_t destID, std::string address);
	bool removeConnection(uint8_t destID);
    
	bool send(uint8_t destID, char messageID[MESSAGE_ID_SIZE], uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	bool recv(uint8_t &sourceID, char messageID[MESSAGE_ID_SIZE], uint8_t buffer[MAX_PACKET_SIZE], uint8_t messageLength);
	
	bool run();
	bool stop();
    
};//end Comms class
#endif//end if COMMS_H
