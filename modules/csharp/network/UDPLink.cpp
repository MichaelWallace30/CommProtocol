#include <network/UDPLink.h>

using namespace Comnet::Network;

/**Constuctor*/
UDPLink::UDPLink(){
	unmanagedUDPLink = new comnet::network::UDPLink();	
}
UDPLink::~UDPLink(){
	if (unmanagedUDPLink != nullptr){
		delete unmanagedUDPLink;
		unmanagedUDPLink = nullptr;
	}
}

/** Opens socket, assigns local address & port, binds socket, sets slen to length of address, sets is connected on scucces/
Returns false if open socket or bind fails*/
Boolean UDPLink::InitConnection( String^ port,  String^ address, uint32_t baudrate){	
	char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
  char* addressChar = nullptr;
  if (address != nullptr) {
	  addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
  }
	return unmanagedUDPLink->InitConnection(portChar, addressChar, baudrate);
}

Boolean UDPLink::InitConnection(String^ port, String^ address){
	return InitConnection(port, address, 0);
}

/** Adds Address & port to destID value of array of aviable connections
Returns false if connection is already connected*/
Boolean UDPLink::AddAddress(uint8_t destID,  String^ address, uint16_t port){
	char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);	
	return unmanagedUDPLink->AddAddress(destID, addressChar, port);
}

/** Sets connection to not available
Returns false is no connection is found*/
Boolean UDPLink::RemoveAddress(uint8_t destID){
	return unmanagedUDPLink->RemoveAddress(destID);
}

/** Sends txData using its length of bytes through the destID connection which is establish through add adress
Return false if no proper connection is establish*/
Boolean UDPLink::Send(uint8_t destID, uint8_t* txData, uint32_t txLength){
	return unmanagedUDPLink->Send(destID, txData, txLength);
}
/** Sets recieved data to rxData and sets the length of the data to rxLength
Returns false if not aviable connection or no data is recieved*/
Boolean UDPLink::Recv(uint8_t* rxData, UInt32% rxLength) { 
  uint32_t length = 0;
	bool success = unmanagedUDPLink->Recv(rxData, &length);//probably wont work
  rxLength = length;
  return success;
}