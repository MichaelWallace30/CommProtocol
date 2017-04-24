#include <CommProto/network/tcplink.h>
#include <CommProto/debug/comms_debug.h>
#include <memory>
#include <CommProto/debug/log.h>

namespace comnet {
	namespace network {
		const int TCPLink::ACCEPT_DELAY_MILLIS = 2000;
		const int TCPLink::RECV_PORT_ATTEMPTS = 10;
		const int TCPLink::RECV_PORT_DELAY_MILLIS = 500;
		const int TCPLink::RECV_PORT_REPLY_ATTEMPTS = 20;
		const int TCPLink::RECV_PORT_REPLY_DELAY_MILLIS = 500;
		const int TCPLink::CON_DELAY_MILLIS = 2000;
		const uint8_t TCPLink::PORT_PAYLOAD_SIZE = 2;
		const uint8_t TCPLink::PORT_REPLY_SIZE = 1;
		const uint8_t TCPLink::ADD_QUEUE_EVENT = 1;
		const uint8_t TCPLink::REMOVE_QUEUE_EVENT = 2;
		const uint8_t TCPLink::PACKET_SIZE_NUM_BYTES = 4;

		TCPLink::~TCPLink()
		{
			free_pointer(acceptThread);
			free_pointer(connectThread);
			if (local != nullptr) {
				local->SockClose();
				delete local;
				local = nullptr;
			}
		}

		bool TCPLink::InitConnection(const char * port, const char * address, uint32_t baud)
		{
			local = CreateTcpSocket();  //Create socket which will be used to accept connections
			uint16_t portNum = std::atoi(port);  //c string to int
			if (local->SockListen(address, portNum) == 0)   //0 means the socket has been successfully set at the specified address and port
			{
				this->localPort = htons(portNum);  //host to network short
				inet_pton(AF_INET, address, &(this->localIP.s_addr));  //converts char ip adderess to standardized INET_ADDRESS
				return true;
			}
			debug::Log::Message(debug::LOG_NOTE, "Could not listen on the specified port and address");
			return false;
		}

		bool TCPLink::AddAddress(uint8_t dest_id, const char * address, uint16_t port)
		{
			clientsMutex.Lock();
			auto conInfo = std::make_pair(dest_id, std::make_shared<TCPHolder>(port, address, dest_id));  //Create pair which stores connection information and the actual connection
			conInfo.second->connect = ShouldConnect(conInfo.second);  //Check if we will be connecting or accepting

			clients.emplace(conInfo);  //Adds client
			clientsMutex.Unlock();
			if (conInfo.second->connect) //if we ShouldConnect(), add the queueEntry to connectModifyQueue
			{
				connectModifyQueueMutex.Lock();
				connectModifyQueue.push(std::make_pair(ADD_QUEUE_EVENT, conInfo.second));
				connectModifyQueueMutex.Unlock();
				connectCondVar.Set();  //Set the Event, so the connect thread starts running
			}
			else  //if we !ShouldConnect(), add the queueEntry to acceptModifyQueue
			{
				acceptModifyQueueMutex.Lock();
				acceptModifyQueue.push(std::make_pair(ADD_QUEUE_EVENT, conInfo.second));
				acceptModifyQueueMutex.Unlock();
				acceptCondVar.Set();  //Set the event so the acceptThread starts running
			}
			return true;
		}

		bool TCPLink::RemoveAddress(uint8_t id)
		{
			CommLock clientLock(clientsMutex);
			auto it = clients.find(id);
			if (it != clients.end()) {
				if (it->second->socket != nullptr)
				{
					if (it->second->connect) //if connect is true, then the TCPHolder will be in the connectQueue
					{
						CommLock lock(connectModifyQueueMutex);
						connectModifyQueue.push(std::make_pair(REMOVE_QUEUE_EVENT, it->second));
					}
					else  //if connect is false, then the TCPHolder will be in the AcceptQueue
					{
						CommLock lock(acceptModifyQueueMutex);
						acceptModifyQueue.push(std::make_pair(REMOVE_QUEUE_EVENT, it->second));
					}
				}
				clients.erase(it);  //Remove from clients
				return true;
			}
			return false;
		}

		bool TCPLink::Send(uint8_t dest_id, uint8_t * tx_data, uint32_t tx_length)
		{
			//TODO: Figure out another place to call this
			clientsMutex.Lock();
			auto it = clients.find(dest_id);
			if (it != clients.end()) {
				TcpPtr tcp = it->second;
				clientsMutex.Unlock();

				if (tcp->socket != nullptr) {
					std::vector <uint8_t> tx_data_extended;
					PacketSizeInt networkLength = htonl((PacketSizeInt)tx_length);
					tx_data_extended.resize(PACKET_SIZE_NUM_BYTES);
					for (int i = 0; i < PACKET_SIZE_NUM_BYTES; i++) {
						tx_data_extended.at(i) = (networkLength >> (8 * i)) & 0xff;
					}
					tx_data_extended.insert(tx_data_extended.end(), &tx_data[0], &tx_data[tx_length]);
					if (tcp->socket->SockSend((const char*)tx_data_extended.data(), tx_data_extended.size()) != 0) //if SockSend returns a nonzero, there was an error
					{
						std::string msg = "Delete TcpSocket for client with ID ";
						msg += std::to_string(dest_id);
						(msg.c_str());
						SetSocket(dest_id, nullptr);
						return false;
					}
					return true;
				}
			}
			else
			{
				clientsMutex.Unlock();
			}
			return false;
		}

		bool TCPLink::Recv(uint8_t * rx_data, uint32_t * rx_length)
		{
			//TODO: Figure out another place to call this
			RunHandlers();
			clientsMutex.Lock();
			auto it = clients.begin();
			while (it != clients.end()) {
				uint8_t id = it->first;
				auto tcp = it->second;
				it++;
				clientsMutex.Unlock();
				if (tcp->socket != nullptr) {
					uint8_t packet_size_data[PACKET_SIZE_NUM_BYTES];
					packet_data_status_t recvStatus = tcp->socket->SockReceive((const char*)packet_size_data, PACKET_SIZE_NUM_BYTES, *rx_length);
					if (recvStatus == PACKET_SUCCESSFUL) {
						PacketSizeInt packSize = 0;
						for (int i = 0; i < PACKET_SIZE_NUM_BYTES; i++) {
							packSize |= (packet_size_data[i] & 0xff) << (8 * i);
						}
						packSize = ntohl(packSize);
						recvStatus = tcp->socket->SockReceive((const char*)rx_data, packSize, *rx_length);
						if (recvStatus == PACKET_SUCCESSFUL) {
							return true;
						}
					}
				}
				clientsMutex.Lock();
			}
			clientsMutex.Unlock();
			return false;
		}

		//Start running the connectHandler and acceptHandler
		bool TCPLink::RunHandlers()
		{
			if (connectThread == nullptr) {
				connectThread = new CommThread(&TCPLink::ConnectHandler, this);
				acceptThread = new CommThread(&TCPLink::AcceptHandler, this);
				connectThread->Detach();
				acceptThread->Detach();
				return true;
			}
			return false;
		}

		//Handles accepting incoming connections
		void TCPLink::AcceptHandler()
		{
			sockaddr_in connectedAddr;  //used later to store the address that connected
			while (true) {
				//Process the acceptModifyQueue to see if any elements must be added or removed
				//This queue exists to prevent the main thread from having to wait for an accepted connection (which can take 10 seconds)
				acceptModifyQueueMutex.Lock();
				while (!acceptModifyQueue.empty()) {
					auto clientPair = acceptModifyQueue.front();
					acceptModifyQueue.pop();
					if (clientPair.first == ADD_QUEUE_EVENT) {
						acceptList.push_back(clientPair.second);
					}
					else
					{
						acceptList.remove(clientPair.second);
					}
				}
				acceptModifyQueueMutex.Unlock();
				if (!acceptList.empty()) {
					CommSocket* socket = local->SockAccept(connectedAddr);  //Checks to see if anyone connected and, if they did, sets the connectAddr to their info
					if (socket != nullptr) {
						bool found = false;
						//The AddressToID isn't a simple address conversion. It is like a second handshake and can take a few seconds.
						uint8_t id = AddressToID(connectedAddr.sin_port, connectedAddr.sin_addr, socket, found);
						if (found) //Indicates if the AddressToID handshake was successful
						{
							LOG_PRINTF(debug::LOG_NOTIFY, "%d accepted!", id);
							SetSocket(id, socket);
							connectedAddr = sockaddr_in();
						}
						else
						{
							char addrChar[50];
							inet_ntop(AF_INET, &(connectedAddr.sin_addr), addrChar, 50);
							LOG_PRINTF(debug::LOG_NOTIFY, "Address: %s  Port: %d tried to connect but was rejected", addrChar, ntohs(connectedAddr.sin_port));
							delete socket;
							socket = nullptr;
						}
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(ACCEPT_DELAY_MILLIS));
				}
				else
				{
					//if nothing is in the AcceptQueue, there is no one to accept, so the thread sleeps
					acceptCondVar.Wait();
				}
			}
		}

		void TCPLink::ConnectHandler()
		{
			while (true) {
				//Process the connectModifyQueue to see if any elements must be added or removed
				//This queue exists to prevent the main thread from having to wait for a connection (which can take 10 seconds)
				connectModifyQueueMutex.Lock();
				while (!connectModifyQueue.empty()) {
					auto clientPair = connectModifyQueue.front();
					connectModifyQueue.pop();
					if (clientPair.first == ADD_QUEUE_EVENT) {
						connectList.push_back(clientPair.second);
					}
					else
					{
						connectList.remove(clientPair.second);
					}
				}
				connectModifyQueueMutex.Unlock();
				if (!connectList.empty()) {
					auto it = connectList.begin();
					while (it != connectList.end()) {
						//Connect can take up to 10 seconds
						if (Connect(*it)) {
							//If connected, there is no reason for the TCPHolder be in the connectQueue, so remove it
							it = connectList.erase(it);
						}
						else
						{
							it++;
						}
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(CON_DELAY_MILLIS));
				}
				else
				{
					//If nothing is in connectList, then there is nothing to connect to, so the thread sleeps
					connectCondVar.Wait();
				}
			}
		}

		//Sets the Socket in the clients dictionary to the specified values. Will requeue disconnected (nullptr) sockets.
		bool TCPLink::SetSocket(uint8_t id, CommSocket * replacement)
		{
			CommLock lock(clientsMutex);
			auto it = clients.find(id);
			if (it != clients.end()) {
				//Creates an entirely new TCPHolder to replace the old TCPHolder, this is to ensure thread safety
				it->second = std::make_shared<TCPHolder>(*clients.at(id), replacement);
				if (replacement == nullptr) {
					if (it->second->connect) //Automatically handles putting requeing disconnected sockets
					{
						connectModifyQueueMutex.Lock();
						connectModifyQueue.push(std::make_pair(ADD_QUEUE_EVENT, it->second));
						connectModifyQueueMutex.Unlock();
						connectCondVar.Set();
					}
					else
					{
						acceptModifyQueueMutex.Lock();
						acceptModifyQueue.push(std::make_pair(ADD_QUEUE_EVENT, it->second));
						acceptModifyQueueMutex.Unlock();
						acceptCondVar.Set();
					}
				}
				return true;
			}
			return false;
		}

		//Called from ConnectHandler, attempts to connect to the address and port in the TCPHolder
		bool TCPLink::Connect(TcpPtr tcp)
		{
			CommSocket* tcpSocket = CreateTcpSocket();
			if (tcpSocket->SockConnect(tcp->addressInput.c_str(), tcp->portInput) == 0) //0 means connection was successful
			{
				//port used to accept connection on this TCPLink instance
				char buffer[PORT_PAYLOAD_SIZE];  //buffer to store the port
				//localPort is already in network byte order
				buffer[0] = localPort & 0xff;
				buffer[1] = (localPort >> 8) & 0xff;
				if (tcpSocket->SockSend(buffer, PORT_PAYLOAD_SIZE) != 0) {
					debug::Log::Message(debug::LOG_WARNING, "Error sending port handshake");
				}
				else
				{
					//Receive the portReply which contains whether their connection was accepted or not
					uint32_t recvSize;
					char recvBuffer[PORT_REPLY_SIZE];
					//Attempt to receive a certain number of times
					for (int i = 0; i < RECV_PORT_REPLY_ATTEMPTS; i++) {
						debug::Log::Message(debug::LOG_NOTE, "looking for port reply");
						if (tcpSocket->SockReceive(recvBuffer, PORT_REPLY_SIZE, recvSize) == PACKET_SUCCESSFUL) {
							if (recvSize == PORT_REPLY_SIZE) {
								//If the number received is greater than 0, the connection was successful
								if ((uint8_t)recvBuffer[0] > 0) {
									std::string msg = "CONNECTED TO ";
									msg += std::to_string((int)tcp->destID);
									LOG_PRINTF(debug::LOG_NOTIFY, "Connected to %d", (int)tcp->destID);
									SetSocket(tcp->destID, tcpSocket);
									return true;
								}
							}
							debug::Log::Message(debug::LOG_WARNING, "Connection failed: Handshake fail");
							break;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(RECV_PORT_REPLY_DELAY_MILLIS));
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
		bool TCPLink::ShouldConnect(TcpPtr tcp)
		{
			/*Use port and address to determine which role (accept or connect) the TCPLink we will be taking
			It is portable and is allows for the same comms id*/
			if (tcp->port < localPort) {
				return true;
			}
			else if (tcp->port > localPort)
			{
				return false;
			}
			else
			{
				return (tcp->address.s_addr < localIP.s_addr);
			}
		}

		//Executes the second accept handshake
		uint8_t TCPLink::AddressToID(USHORT port, IN_ADDR address, CommSocket* socket, bool & success)
		{
			//ConnectedListenPort will store the port sent by the connector in case there are other sockets on a matching address, so we don't have to ask for the port again
			uint16_t connectedListenPort = 0;
			//Iterate over acceptList to see if the address that is connecting matches an address we should be accepting
			for (auto it = acceptList.begin(); it != acceptList.end(); it++) {
				TcpPtr tcp = *it;
				if (address.s_addr == tcp->address.s_addr)
				{
					if (connectedListenPort == 0) {
						char buffer[PORT_PAYLOAD_SIZE];
						uint32_t size = 0;
						for (int i = 0; i < RECV_PORT_ATTEMPTS; i++) {
							if (socket->SockReceive(buffer, PORT_PAYLOAD_SIZE, size) == PACKET_SUCCESSFUL) {
								if (size == PORT_PAYLOAD_SIZE) {
									connectedListenPort = buffer[0] & 0xff;
									connectedListenPort |= buffer[1] << 8;
								}
								break;
							}
							std::this_thread::sleep_for(std::chrono::milliseconds(RECV_PORT_DELAY_MILLIS));
						}
					}
					//If the connectedListenPort matches the current TCPHolder
					if (connectedListenPort == tcp->port) {
						//Send a success code the connector
						char buffer[PORT_REPLY_SIZE];
						buffer[0] = 0x01;
						if (socket->SockSend(buffer, PORT_REPLY_SIZE) != 0)
						{
							debug::Log::Message(debug::LOG_WARNING, "Sending handshake reply failed");
							success = false;
							return false;
						}
						success = true;
						/*No longer accept other connections (this is debatable we may want
						to continue accepting and overwrite current, possibly invalid,
						connections, this is really only a problem if you are not sending
						data often as that is how you detect if a connection is broken)
						*/
						acceptList.erase(it);
						return tcp->destID;
					}
				}
			}
			//Send a fail code if the port could not be found
			char buffer[PORT_REPLY_SIZE];
			buffer[0] = 0x00;
			socket->SockSend(buffer, PORT_REPLY_SIZE);
			success = false;
			return 0;
		}

		//A copy constructor for TCPHolder to use when changing sockets
		TCPHolder::TCPHolder(TCPHolder & copy, CommSocket * replacement)
			:socket(replacement), addressInput(copy.addressInput),
			address(copy.address), portInput(copy.portInput),
			port(copy.port), connect(copy.connect),
			destID(copy.destID)
		{
		}

		TCPHolder::TCPHolder(uint16_t port, const char * addr, uint8_t destID, CommSocket * socket)
			:socket(socket), destID(destID), portInput(port), addressInput(addr)
		{
			this->port = htons(port);
			inet_pton(AF_INET, addr, &(address.s_addr));
		}

		TCPHolder::~TCPHolder() {
			if (socket != nullptr) {
				/*If we connected, the socket used is different from the acceptor socket and can be safely closed
				If we accepted then closing the socket would prevent us from accepting other connections*/
				if (this->connect) {
					socket->SockClose();
				}
				delete socket;
				socket = nullptr;
			}
		}
	}
}