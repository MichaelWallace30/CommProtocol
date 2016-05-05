#include <stdint.h>

#define MAX_PACKET_SIZE 1024
#define ENCRYPTION_BLOCK_BYTES 16
#define MESSAGE_ID_SIZE 4
#define MAX_CONNECTIONS 25

/** include encryption header*/
#include "aes.h"
using CryptoPP::AES;

#ifndef COMMS_H
#define COMMS_H

/** C# wrapper can not load fstream, to fix this problem if windows, then
use managed c++ code to open and read file*/
#ifdef _MANAGED
    #using<system.dll>
    using namespace System;
    using namespace System::IO;
    using namespace System::Runtime::InteropServices;
#else
    #include <fstream>
    #include <string>
#endif


class Comms{
   

private:
    /**Add key to comnet class*/
    uint8_t key[AES::DEFAULT_KEYLENGTH];
    /** Method to read key form text file*/
    void loadKey();
    
    //read me Mario
    //queue maybe two queues one for send and one for recive?
    
    //array of connections to match destID with address use max connections
   
   
   //read me Mario
   //thread methods
        //recv: recv,build packet,  decrypt, and queue  
        //send: deque , encrypt, seralize, send
   
    
    
public:
    /** Constuctor */
    Comms();
    /** Destcutor */
    ~Comms();
    /** Packet header stcture for sending messages*/
    struct header_t
    {
        uint8_t destID;
        uint8_t sourceID;
        uint8_t messaeID[MESSAGE_ID_SIZE];
        uint8_t IV[ENCRYPTION_BLOCK_BYTES];//random init vector for encryption
        uint16_t messageLength;    
    }

    /** Packet structure uses header strcut and fixed data size */
    struct packet_t
    {
        header_t header;
        uint8_t data[MAX_PACKET_SIZE];    
    }
    
    //init connections(port, baud rate = "")
    //add connections(id, address)
    //remove connections
    //send  queue
    //recv  dequeue
    //run  start threads
    //stop stop threads

    
    

};//end Comms class
#endif//end if COMMS_H
