/*
  Standard Comms Node.

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMS_H
#define COMMS_H

#include <CommProto/tools/data_structures/interface/interface_queue.h>
#include <CommProto/debug/error_status.h>
#include <CommProto/console/console.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/comm_condvar.h>
#include <CommProto/network/commslink.h>
#include <CommProto/commnode.h>
#include <CommProto/encryption/encryptor.h>
#include <CommProto/encryption/decryptor.h>
#include <CommProto/constate/connection_state_manager.h>
#include <memory>


namespace comnet {
  

using namespace std;
using namespace comnet::tools::datastructures;
using namespace comnet::serialization;
using namespace comnet::network;	
using namespace comnet::architecture::os;
using namespace comnet::constate;
  /**
    Comms is a standard CommNode node object. It handles elementary and intermediate 
    commands and functionality in order to work to the user's specifications of communications.
  */  
  _COMNET_PUBLIC_API_ class COMM_EXPORT Comms : public CommNode {
private:
 CommMutex send_mutex;
 CommMutex recv_mutex;
 CommMutex console_mutex;
 CommConditionVariable comm_cond_var_send;
 
 
 /** AES encryption class for encrypting and decrypting buffer stream not header*/
 //AesEncryption  aes_encryption;

 /** Length of data buffer for communication stream */
 uint32_t rx_length;

 /** Thread to Run communication data */
 CommThread comm_thread_send;
 CommThread comm_thread_recv;
 CommThread console_thread;

 /** Method to Run in communication thread */
 void CommunicationHandlerSend();
 void CommunicationHandlerRecv();

 /** Polymorphic (Base Class) Communication link for connection code*/
 CommsLink *conn_layer;


 /** Encryption class used to encrypt and dectrypt data */
  encryption::CommEncryptor encrypt;
  encryption::CommDecryptor decrypt;

  /** Used to check if remote comms are active or inactive. */
  std::shared_ptr <ConnectionStateManager> conStateManager;

public:		
 /** Constructor */
 Comms(uint8_t platform_id);
 /** Destructor */
 ~Comms();

 /** input c string as the form of encrytion key*/
 bool LoadKey(char* key);
 /** load file which contatins the encryption key by the file name*/
 bool LoadKeyFromFile(char*keyFileName);

 /** Method will init the connection device and return false if connection failed to init properly
 enum connection type to use serial, UDP, zigbee, ect
 port number is the comport or UDP port used will differ from windows and Unix for comports COM05 or /dev/ttyUSB05
 baud-rate is not used for for UDP which is not needed but for serial and zigbee it is needed for baud rate */
 bool InitConnection(transport_protocol_t conn_type, 
   const char* port, 
   const char* address = NULL, 
   uint32_t baudrate = 0) override;

 /** The address entered will be paired for communication by destination ID
 Adding address can be a UDP IPV4 or hex MAC address for zigbee
 Adding an address is not need for serial and will default to ""*/
 bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0) override;

 /** Removing an address removes the known association of address and destination ID by using id*/
 bool RemoveAddress(uint8_t dest_id) override;

 bool Send(AbstractPacket& packet, uint8_t dest_id) override;

 bool ReplaceSendQueue(const Queue<ObjectStream*>* queue) override;

 bool ReplaceReceiveQueue(const Queue<std::pair<uint8_t, AbstractPacket*>>* queue);

 AbstractPacket* Receive(uint8_t&  source_id) override;

 /** Method to start communication*/
 void Run() override;
 /** Method to toggle Pause communication*/
 void Pause() override;
 /** Method to Stop communication*/
 void Stop() override;
 // Sets up the home console.
 bool SetupConsole(uint16_t port, const char* addr = nullptr) { return false; }

 /** Accessor for {@link #pingManager}.*/
 std::shared_ptr <ConnectionStateManager> GetConStateManager() {
   return conStateManager;
 }

protected:
 // Nothing yet.
 void LogToConsoles();
private:

  void HandlePacket(error_t error, uint8_t sourceID, AbstractPacket* packet);
 };//End Comms class      
} // namespace Comnet
#endif//End if COMMS_H
