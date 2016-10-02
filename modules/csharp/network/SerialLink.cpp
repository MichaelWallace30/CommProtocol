#include <network/SerialLink.h>
#include <CommProto/debug/comms_debug.h>


using namespace Comnet::Network;

SerialLink::SerialLink(){
	unmanagedSerialLink = new comnet::network::SerialLink();
}
SerialLink::~SerialLink(){
	if (unmanagedSerialLink != nullptr){
		delete unmanagedSerialLink;
		unmanagedSerialLink = nullptr;
	}
}
/** Opens comport sets if it is connected on scucces, address is not need just use "" argument
Returns false if opening comport fails*/
Boolean SerialLink::InitConnection(String^ port, String^ address, uint32_t baudrate){
	char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
	return unmanagedSerialLink->InitConnection(portChar, "", baudrate);
}
/**  initConncetion)*/
Boolean SerialLink::InitConnection(String^ port, uint32_t baudrate){
	return InitConnection(port, baudrate);
}
/** Add serial address returns true does nothing */
Boolean SerialLink::AddAddress(uint8_t destID, String^ address, uint16_t port){
	//going to pass arguments in probably will never be used.
	char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
	return unmanagedSerialLink->AddAddress(destID, addressChar, port);
}
/** Remove serial address returns true does nothing */
Boolean SerialLink::RemoveAddress(uint8_t destID){
	return unmanagedSerialLink->RemoveAddress(destID);
}
/**
Sends txData using its length of bytes through the serial connection. Connection is broadcast
destID is only used for packing / unpacking. Return false if no proper connection is establish
*/
Boolean SerialLink::Send(uint8_t destID, uint8_t* txData, uint32_t txLength){
	return unmanagedSerialLink->Send(destID, txData, txLength);
}
/** Sets recieved data to rxData and sets the length of the data to rxLength
Returns false if not aviable connection, No data is recieved, or time out*/
bool SerialLink::Recv(uint8_t* rxData, uint32_t% rxLength){
  uint32_t length = 0;
	bool success = unmanagedSerialLink->Recv(rxData, &length);//mgiht cause errors c cast (uint32_t*)
  rxLength = length;
  return success;
}
