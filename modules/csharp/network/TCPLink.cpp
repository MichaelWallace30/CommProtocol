#include <network/TCPLink.h>
#include <algorithm>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/headerpacket.h>
#include <string>
#include <vector>

namespace Comnet
{
	namespace Network {
		using namespace comnet::debug;
		TCPLink::TCPLink() {
			connectList = gcnew LinkedList<TCPHolder^>();
			connectModifyQueue = gcnew ConcurrentQueue<QueueEntry^>();
			connectRE = gcnew AutoResetEvent(false);

			acceptList = gcnew LinkedList<TCPHolder^>();
			acceptModifyQueue = gcnew ConcurrentQueue<QueueEntry^>();
			acceptRE = gcnew AutoResetEvent(false);

			clients = gcnew ConcurrentDictionary<uint8_t, TCPHolder^>();

			localIP = new IN_ADDR();
		}

		TCPLink::~TCPLink() {
			this->!TCPLink();
		}

		TCPLink::!TCPLink() {
			if (localIP != nullptr) {
				delete localIP;
				localIP = nullptr;
			}
			if (local != nullptr) {
				local->SockClose();
				delete local;
				local = nullptr;
			}
		}

		Boolean TCPLink::InitConnection(String ^ port, String^ address, uint32_t baudrate)
		{
			char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);  //convert c# string to c string
			char* addressChar = nullptr;  //will store c_string version of address
			if (address != nullptr) {
				addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);  //converts c# string to c string
			}
			local = comnet::network::CreateTcpSocket();  //Creates a TcpSocket which will be used to accept incoming connections
			uint16_t portNum = std::atoi(portChar);  //Converts string to int
			if (local->SockListen(addressChar, portNum) == 0) //Tells socket to accept incoming connections at the specified address & port (0 is success)
			{
				this->localPort = htons(portNum);  //host to network short
				inet_pton(AF_INET, addressChar, &(this->localIP->s_addr));  //converts char ip adderess to standardized INET_ADDRESS
				return true;
			}
			COMMS_DEBUG("Could not listen on the specified port and address");
			return false;
		}

		Boolean TCPLink::AddAddress(uint8_t destID, String ^ address, uint16_t port)
		{
			char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);  //convert C# string to c string
			QueueEntry^ entry = gcnew QueueEntry();  //Queue Entries contain the queue action and the TCPHolder to add
			entry->qEvent = ADD_QUEUE_EVENT;  //Tells the Connect or Accept handlers we are adding this TCPHolder
			entry->tcp = gcnew TCPHolder(port, address, destID, nullptr);  //Create tcp holder which stores connection information and the actual connection
			entry->tcp->connect = ShouldConnect(entry->tcp);  //Check if we will be connecting or accepting
			if (!clients->TryAdd(destID, entry->tcp)) //Attempts to add TCPHolder to the clients dictionary
			{
				COMMS_DEBUG("TCPLink: Could not add client (already exists?)");
				return false;
			}
			if (entry->tcp->connect) //if we ShouldConnect(), add the queueEntry to connectModifyQueue
			{
				connectModifyQueue->Enqueue(entry);
				connectRE->Set(); //Set the Event, so the connect thread starts running
			}
			else  //if we !ShouldConnect(), add the queueEntry to acceptModifyQueue
			{
				acceptModifyQueue->Enqueue(entry);
				acceptRE->Set();  //Set the event so the acceptThread starts running
			}
			return true;
		}

		Boolean TCPLink::RemoveAddress(uint8_t destID)
		{
			TCPHolder^ tcp;
			if (clients->TryGetValue(destID, tcp)) {
				if (tcp->socket != nullptr) {
					QueueEntry^ entry = gcnew QueueEntry();
					entry->qEvent = REMOVE_QUEUE_EVENT;  //Indicates we are removing the element
					entry->tcp = tcp;  //Set the TCPHolder that should be removed
					if (tcp->connect) //if connect is true, then the TCPHolder will be in the connectQueue
					{
						connectModifyQueue->Enqueue(entry);
					}
					else  //if connect is false, then the TCPHolder will be in the AcceptQueue
					{
						acceptModifyQueue->Enqueue(entry);
					}
				}
				if (!clients->TryRemove(destID, tcp))  //Remove the tcpHolder from clients
				{
					COMMS_DEBUG("TCPLink::RemoveAddress tryRemove on clients failed");
					return false;
				}
				return true;
			}
			return false;
		}

		Boolean TCPLink::Send(uint8_t destID, uint8_t* txData, uint32_t txLength)
		{
			TCPHolder^ tcp;
			if (clients->TryGetValue(destID, tcp)) {
				if (tcp->socket != nullptr) {
					std::vector <uint8_t> tx_data_extended;
					PacketSizeInt networkLength = htonl((PacketSizeInt)txLength);
					tx_data_extended.resize(PACKET_SIZE_NUM_BYTES);
					for (int i = 0; i < PACKET_SIZE_NUM_BYTES; i++) {
						tx_data_extended.at(i) = (networkLength >> (8 * i)) & 0xff;
					}
					tx_data_extended.insert(tx_data_extended.end(), &txData[0], &txData[txLength]);
					if (tcp->socket->SockSend((const char*)tx_data_extended.data(), tx_data_extended.size()) != 0) //if SockSend returns a nonzero, there was an error
					{
						String^ msg = gcnew String("Delete TcpSocket for client with ID ");
						msg->Concat(System::Convert::ToString((int)destID));
						char* msgChar = (char*)(void*)Marshal::StringToHGlobalAnsi(msg);
						COMMS_DEBUG(msgChar);
						SetSocket(destID, nullptr);
						return false;
					}
					return true;
				}
			}
			return false;
		}

		Boolean TCPLink::Recv(uint8_t* rxData, UInt32% rxLength)
		{
			//TODO: Figure out another place to call this
			RunHandlers();  //The Accept and Connect handlers are run once the first call to Recv is made (this should be changed)
			auto iter = clients->GetEnumerator();
			while (iter->MoveNext()) //Iterate over clients
			{
				TCPHolder^ tcp = iter->Current.Value;
				if (tcp->socket != nullptr) //Check if a socket is open
				{
					uint32_t recvSize = 0;

					uint8_t packet_size_data[PACKET_SIZE_NUM_BYTES];
					comnet::network::packet_data_status_t recvStatus = tcp->socket->SockReceive((const char*)packet_size_data, PACKET_SIZE_NUM_BYTES, recvSize);
					if (recvStatus == comnet::network::PACKET_SUCCESSFUL) {
						PacketSizeInt packSize = 0;
						for (int i = 0; i < PACKET_SIZE_NUM_BYTES; i++) {
							packSize |= (packet_size_data[i] & 0xff) << (8 * i);
						}
						packSize = ntohl(packSize);
						recvStatus = tcp->socket->SockReceive((const char*)rxData, packSize, recvSize);
						if (recvStatus == comnet::network::PACKET_SUCCESSFUL) {
							rxLength = recvSize;
							return true;
						}
					}
				}
			}
			return false;
		}

		//Start running the connectHandler and acceptHandler
		Boolean TCPLink::RunHandlers()
		{
			if (connectThread == nullptr) {
				connectThread = gcnew Thread(gcnew ThreadStart(this, &TCPLink::ConnectHandler));
				connectThread->Start();
				acceptThread = gcnew Thread(gcnew ThreadStart(this, &TCPLink::AcceptHandler));
				acceptThread->Start();
				return true;
			}
			return false;
		}

		//Accepts incoming connections
		Void TCPLink::AcceptHandler()
		{
			//used later to store the address that connected
			sockaddr_in connectedAddr;
			while (true) {
				//Process the acceptModifyQueue to see if any elements must be added or removed
				//This queue exists to prevent the main thread from having to wait for an accepted connection (which can take 10 seconds)
				QueueEntry^ entry;
				while (acceptModifyQueue->TryDequeue(entry)) {
					if (entry->qEvent == ADD_QUEUE_EVENT) {
						acceptList->AddFirst(entry->tcp);
					}
					else
					{
						if (!acceptList->Remove(entry->tcp)) {
							COMMS_DEBUG("Could not remove tcp from acceptList");
						}
					}
				}

				if (acceptList->Count > 0) {
					comnet::network::CommSocket* socket = local->SockAccept(connectedAddr);  //Checks to see if anyone connected and, if they did, sets the connectAddr to their info
					if (socket != nullptr) //socket is not null when someone has connected
					{
						bool found = false;
						//The AddressToID isn't a simple address conversion. It is like a second handshake and can take a few seconds.
						uint8_t id = AddressToID(connectedAddr.sin_port, connectedAddr.sin_addr, socket, found);
						if (found) //Indicates if the AddressToID handshake was successful
						{
							SetSocket(id, socket);
							String^ msg = gcnew String(System::Convert::ToString((int)id));
							msg->Concat(" ACCEPTED");
							char* msgChar = (char*)(void*)Marshal::StringToHGlobalAnsi(msg);
							COMMS_DEBUG(msgChar);
						}
						else
						{
							char addrChar[50];
							inet_ntop(AF_INET, &(connectedAddr.sin_addr), addrChar, 50);
							std::string msg = "Address: ";
							msg += addrChar;
							msg += " Port: ";
							msg += ntohs(connectedAddr.sin_port);
							msg += " tried to connect but was rejected";
							COMMS_DEBUG(msg.c_str());
						}
					}
					Sleep(ACCEPT_DELAY_MILLIS);
				}
				else
				{
					//if nothing is in the AcceptQueue, there is no one to accept, so the thread sleeps
					acceptRE->WaitOne();
				}
			}
		}

		Void TCPLink::ConnectHandler() {
			while (true) {
				//Process the connectModifyQueue to see if any elements must be added or removed
				//This queue exists to prevent the main thread from having to wait for a connection (which can take 10 seconds)
				QueueEntry^ entry;
				while (connectModifyQueue->TryDequeue(entry)) {
					if (entry->qEvent == ADD_QUEUE_EVENT) {
						connectList->AddFirst(entry->tcp);
					}
					else
					{
						if (!connectList->Remove(entry->tcp)) {
							COMMS_DEBUG("Could not remove tcp from acceptList");
						}
					}
				}

				if (connectList->Count > 0) {
					auto it = connectList->GetEnumerator();
					for (LinkedListNode<TCPHolder^>^ it = connectList->First; it != nullptr; )
					{
						LinkedListNode<TCPHolder^>^ next = it->Next;  //must set to next now in case of removal
						//Connect can take up to 10 seconds
						if (Connect(it->Value)) {
							//If connected, there is no reason for the TCPHolder be in the connectQueue, so remove it
							connectList->Remove(it);
						}
						it = next;
					}
					Sleep(CON_DELAY_MILLIS);
				}
				else
				{
					//If nothing is in connectList, then there is nothing to connect to, so the thread sleeps
					connectRE->WaitOne();
				}
			}
		}

		//Sets the Socket in the TCPHolder in the clients dictionary to the specified values. Will requeue disconnected (nullptr) sockets.
		Boolean TCPLink::SetSocket(uint8_t id, comnet::network::CommSocket* replacement) {
			TCPHolder^ tcp;
			if (clients->TryGetValue(id, tcp))
			{
				//Creates an entirely new TCPHolder to replace the old TCPHolder, this is to ensure thread safety
				TCPHolder^ newHolder = gcnew TCPHolder(tcp, replacement);
				clients[id] = newHolder;
				if (replacement == nullptr) //Automatically handles putting requeing disconnected sockets
				{
					QueueEntry^ entry = gcnew QueueEntry();
					entry->qEvent = ADD_QUEUE_EVENT;
					entry->tcp = newHolder;
					if (entry->tcp->connect) {
						connectModifyQueue->Enqueue(entry);
						connectRE->Set();
					}
					else
					{
						acceptModifyQueue->Enqueue(entry);
						acceptRE->Set();
					}
				}
				return true;
			}
			return false;
		}

		//Called from ConnectHandler, attempts to connect to the address and port in the TCPHolder
		Boolean TCPLink::Connect(TCPHolder^ tcp)
		{
			comnet::network::CommSocket* tcpSocket = comnet::network::CreateTcpSocket();
			char* addrChar = (char*)(void*)Marshal::StringToHGlobalAnsi(tcp->addressInput);
			if (tcpSocket->SockConnect(addrChar, tcp->portInput) == 0)  //0 means connection was successful
			{
				//port used to accept connection on this TCPLink instance
				std::vector <char> sendBuffer;  //buffer to store the port
				sendBuffer.resize(PORT_PAYLOAD_SIZE);
				//localPort is already in network byte order
				sendBuffer[0] = localPort & 0xff;
				sendBuffer[1] = (localPort >> 8) & 0xff;
				if (tcpSocket->SockSend(sendBuffer.data(), PORT_PAYLOAD_SIZE) != 0)
				{
					COMMS_DEBUG("Error sending port handshake");
				}
				else
				{
					//Receive the portReply which contains whether their connection was accepted or not
					uint32_t recvSize;
					std::vector <char> recvBuffer;
					recvBuffer.resize(PORT_REPLY_SIZE);
					//Attempt to receive a certain number of times
					for (int i = 0; i < RECV_PORT_REPLY_ATTEMPTS; i++) {
						if (tcpSocket->SockReceive(recvBuffer.data(), PORT_REPLY_SIZE, recvSize) == comnet::network::PACKET_SUCCESSFUL) {
							if (recvSize == PORT_REPLY_SIZE) {
								//If the number received is greater than 0, the connection was successful
								if ((uint8_t)recvBuffer[0] > 0) {
									std::string msg = "CONNECTED TO ";
									msg += std::to_string((int)tcp->destID);
									COMMS_DEBUG(msg.c_str());
									SetSocket(tcp->destID, tcpSocket);
									return true;
								}
							}
							COMMS_DEBUG("Connection failed: Handshake fail");
							break;
						}
						Sleep(RECV_PORT_REPLY_DELAY_MILLIS);
					}
				}
			}
			//If we have not returned, we have not connected, so delete the tcpSocket we attempted to connect with
			tcpSocket->SockClose();
			delete tcpSocket;
			tcpSocket = nullptr;
			return false;
		}

		//If true, we should be trying to connect to the TCP Socket, if false we should try to accept it
		Boolean TCPLink::ShouldConnect(TCPHolder^ tcp)
		{
			/*Use port and address to determine which role (accept or connect) the TCPLink we will be taking
			It is portable and is allows for the same comms id*/
			if (tcp->port < localPort)
			{
				return true;
			}
			else if (tcp->port > localPort)
			{
				return false;
			}
			else
			{
				return (tcp->address->s_addr < localIP->s_addr);
			}
		}

		//Executes the second accept handshake
		uint8_t TCPLink::AddressToID(USHORT port, IN_ADDR address, comnet::network::CommSocket* socket, bool% success)
		{
			//ConnectedListenPort will store the port sent by the connector in case there are other sockets on a matching address, so we don't have to ask for the port again
			uint16_t connectedListenPort = 0;
			acceptList->First;
			//Iterate over acceptList to see if the address that is connecting matches an address we should be accepting
			for (LinkedListNode<TCPHolder^>^ it = acceptList->First; it != nullptr; )
			{
				LinkedListNode<TCPHolder^>^ next = it->Next;
				TCPHolder^ tcp = it->Value;
				if (address.s_addr == tcp->address->s_addr)
				{
					if (connectedListenPort == 0)
					{
						std::vector<char> recvBuffer;
						recvBuffer.resize(PORT_PAYLOAD_SIZE);
						uint32_t recvSize = 0;
						for (int i = 0; i < RECV_PORT_ATTEMPTS; i++)
						{
							if (socket->SockReceive(recvBuffer.data(), PORT_PAYLOAD_SIZE, recvSize) == comnet::network::PACKET_SUCCESSFUL) {
								if (recvSize == PORT_PAYLOAD_SIZE)
								{
									connectedListenPort = recvBuffer[0] & 0xff;
									connectedListenPort |= recvBuffer[1] << 8;
								}
								break;
							}
							Sleep(RECV_PORT_DELAY_MILLIS);
						}
					}
					//If the connectedListenPort matches the current TCPHolder
					if (connectedListenPort == tcp->port) {
						//Send a success code the connector
						std::vector <char> sendBuffer;
						sendBuffer.resize(PORT_REPLY_SIZE);
						sendBuffer[0] = 0x01;
						if (socket->SockSend(sendBuffer.data(), PORT_REPLY_SIZE) != 0)
						{
							COMMS_DEBUG("Sending handshake reply failed");
							success = false;
							return false;
						}
						success = true;
						/*No longer accept other connections (this is debatable we may want
						to continue accepting and overwrite current, possibly invalid,
						connections, this is really only a problem if you are not sending
						data often as that is how you detect if a connection is broken)
						*/
						acceptList->Remove(it);
						return tcp->destID;
					}
				}
				it = next;
			}
			//Send a fail code if the port could not be found
			std::vector <char> sendBuffer;
			sendBuffer.resize(PORT_REPLY_SIZE);
			sendBuffer[0] = 0x00;
			socket->SockSend(sendBuffer.data(), PORT_REPLY_SIZE);
			success = false;
			return 0;
		}

		//A copy constructor for TCPHolder to use when changing sockets
		TCPHolder::TCPHolder(TCPHolder^ copy, comnet::network::CommSocket* replacement)
			:socket(replacement), addressInput(copy->addressInput),
			address(copy->address), portInput(copy->portInput),
			port(copy->port), connect(copy->connect),
			destID(copy->destID)
		{
			this->address = new IN_ADDR();
			char* addrChar = (char*)(void*)Marshal::StringToHGlobalAnsi(copy->addressInput);
			inet_pton(AF_INET, addrChar, &(this->address->s_addr));
		}

		TCPHolder::TCPHolder(uint16_t port, String^ addr, uint8_t destID, comnet::network::CommSocket* socket)
			:socket(socket), destID(destID), portInput(port), addressInput(addr)
		{
			this->address = new IN_ADDR();
			this->port = htons(port);
			char* addrChar = (char*)(void*)Marshal::StringToHGlobalAnsi(addr);
			inet_pton(AF_INET, addrChar, &(this->address->s_addr));
		}

		TCPHolder::!TCPHolder()
		{
			if (address != nullptr)
			{
				delete address;
				address = nullptr;
			}
			if (socket != nullptr)
			{
				/*If we connected, the socket used is different from the acceptor socket and can be safely closed
				If we accepted then closing the socket would prevent us from accepting other connections*/
				if (connect)
				{
					socket->SockClose();
				}
				delete socket;
				socket = nullptr;
			}
		}

		TCPHolder::~TCPHolder()
		{
			this->!TCPHolder();
		}
	}
}