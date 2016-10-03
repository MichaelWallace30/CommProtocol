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

#include <CommProto/tools/data_structures/auto_queue.h>//marios wrapper of queue
#include <CommProto/tools/data_structures/interface/interface_queue.h>

#include <CommProto/network/commslink.h> //communication layer interface/abstract base class
#include <CommProto/network/commsocket.h> // 

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/os_threads.h>//method to Create threads
#include <CommProto/architecture/os/os_mutex.h>
#include <CommProto/architecture/macros.h>//str_lgnth(char*, int)

#include <CommProto/serialization/objectstream.h>

#include <CommProto/abstractpacket.h>
#include <CommProto/commnode.h>
#include <CommProto/headerpacket.h>//Header which declares packet structs

#include <iostream>//testing only
#include <stdint.h>//needed for bit size variables


namespace comnet {
  

  using namespace std;
  using namespace comnet::tools::datastructures;
  using namespace comnet::serialization;
  using namespace comnet::network;	
  using namespace comnet::architecture::os;
  /**
    Comms is a standard CommNode node object. It handles elementary and intermediate 
    commands and functionality in order to work to the user's specifications of communications.
  */  
  _COMNET_PUBLIC_API_ class Comms : public CommNode {
	private:
		mutex_t send_mutex;
		mutex_t recv_mutex;

		/** Encryption key*/
		uint8_t key[KEY_LENGTH];
		/** Method to read key form text file*/
		void LoadKey();

		/** Length of data buffer for communication stream */
		uint32_t rx_length;

		/** Thread to Run communication data */
		CommThread comm_thread_send;
		CommThread comm_thread_recv;

		/** Method to Run in communication thread */
		void CommunicationHandlerSend();
		void CommunicationHandlerRecv();

		/** Polymorphic (Base Class) Communication link for connection code*/
		CommsLink *conn_layer;


	public:		
		/** Constructor */
		Comms(uint8_t platform_id);
		/** Destructor */
		~Comms();

		/** Method will init the connection device and return false if connection failed to init properly
		enum connection type to use serial, UDP, zigbee, ect
		port number is the comport or UDP port used will differ from windows and Unix for comports COM05 or /dev/ttyUSB05
		baud-rate is not used for for UDP which is not needed but for serial and zigbee it is needed for baud rate */
		bool InitConnection(transport_protocol_t conn_type, const char* port, const char* address = NULL, uint32_t baudrate = 0);

		/** The address entered will be paired for communication by destination ID
		Adding address can be a UDP IPV4 or hex MAC address for zigbee
		Adding an address is not need for serial and will default to ""*/
		bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0);

		/** Removing an address removes the known association of address and destination ID by using id*/
		bool RemoveAddress(uint8_t dest_id);

		bool Send(AbstractPacket* packet, uint8_t dest_id);
		AbstractPacket* Receive(uint8_t&  source_id);

		/** Method to start communication*/
		void Run();
		/** Method to toggle Pause communication*/
		void Pause();
		/** Method to Stop communication*/
		void Stop();

	protected:
		// Nothing yet.
		void LogToConsoles()
		{ }
	};//End Comms class      
} // namespace Comnet
#endif//End if COMMS_H
