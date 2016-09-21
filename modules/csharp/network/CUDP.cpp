#include <network/CUDP.h>

using namespace System::Runtime::InteropServices;
using namespace Comnet::Network;

/**Constuctor*/
CUDP::CUDP(){
	unmanagedUDP = new UDP();	
}
CUDP::~CUDP(){
	if (unmanagedUDP != nullptr){
		delete unmanagedUDP;
		unmanagedUDP = nullptr;
	}
}

/** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
Returns false if open socket or bind fails*/
Boolean CUDP::initConnection( String^ port,  String^ address, uint32_t baudrate){	
	char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
	char* addresstChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
	return unmanagedUDP->initConnection(portChar, addresstChar, baudrate);
}

/** Adds Address & port to destID value of array of aviable connections
Returns false if connection is already connected*/
Boolean CUDP::addAddress(uint8_t destID,  String^ address, uint16_t port){
	char* addresstChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);	
	return unmanagedUDP->addAddress(destID, addresstChar, port);
}

/** Sets connection to not available
Returns false is no connection is found*/
Boolean CUDP::removeAddress(uint8_t destID){
	return unmanagedUDP->removeAddress(destID);
}

/** Sends txData using its length of bytes through the destID connection which is establish through add adress
Return false if no proper connection is establish*/
Boolean CUDP::send(uint8_t destID, uint8_t* txData, uint32_t txLength){
	return unmanagedUDP->send(destID, txData, txLength);
}
/** Sets recieved data to rxData and sets the length of the data to rxLength
Returns false if not aviable connection or no data is recieved*/
Boolean CUDP::recv(uint8_t* rxData, UInt32% rxLength){
	return unmanagedUDP->recv(rxData, (uint32_t*)rxLength);//probably wont work
}