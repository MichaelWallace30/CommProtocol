#include <network/CUDPLink.h>

using namespace Comnet::Network;

/**Constuctor*/
CUDPLink::CUDPLink(){
	unmanagedUDPLink = new UDPLink();	
}
CUDPLink::~CUDPLink(){
	if (unmanagedUDPLink != nullptr){
		delete unmanagedUDPLink;
		unmanagedUDPLink = nullptr;
	}
}

/** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
Returns false if open socket or bind fails*/
Boolean CUDPLink::initConnection( String^ port,  String^ address, uint32_t baudrate){	
	char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
	char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
	return unmanagedUDPLink->initConnection(portChar, addressChar, baudrate);
}

Boolean CUDPLink::initConnection(String^ port, String^ address){
	return initConnection(port, address, 0);
}

/** Adds Address & port to destID value of array of aviable connections
Returns false if connection is already connected*/
Boolean CUDPLink::addAddress(uint8_t destID,  String^ address, uint16_t port){
	char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);	
	return unmanagedUDPLink->addAddress(destID, addressChar, port);
}

/** Sets connection to not available
Returns false is no connection is found*/
Boolean CUDPLink::removeAddress(uint8_t destID){
	return unmanagedUDPLink->removeAddress(destID);
}

/** Sends txData using its length of bytes through the destID connection which is establish through add adress
Return false if no proper connection is establish*/
Boolean CUDPLink::send(uint8_t destID, uint8_t* txData, uint32_t txLength){
	return unmanagedUDPLink->send(destID, txData, txLength);
}
/** Sets recieved data to rxData and sets the length of the data to rxLength
Returns false if not aviable connection or no data is recieved*/
Boolean CUDPLink::recv(uint8_t* rxData, UInt32% rxLength){
	return unmanagedUDPLink->recv(rxData, (uint32_t*)rxLength);//probably wont work
}