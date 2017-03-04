#include <CommProto/network/tcplink.h>
#include <CommProto/debug/comms_debug.h>

namespace comnet {

		namespace network {
				TCPLink::~TCPLink()
				{
						free_pointer(acceptThread);
						free_pointer(connectThread);
						free_pointer(local);
				}

				bool TCPLink::InitConnection(const char * port, const char * address, uint32_t commID)
				{
						local = CreateTcpSocket();
						this->localPort = std::atoi(port);
						local->SockListen(address, localPort);
						this->commID = commID;
						return true;
				}

				bool TCPLink::AddAddress(uint8_t dest_id, const char * address, uint16_t port)
				{
						connectQueueMutex.Lock();
						connectQueue.push_back(std::make_pair(dest_id, std::make_shared<TCPHolder>(port, address)));
						connectQueueMutex.Unlock();
						connectCondVar.Set();
						return true;
				}

				bool TCPLink::RemoveAddress(uint8_t id)
				{
						{
								CommLock clientLock(clientsMutex);
								{
										auto it = clients.find(id);
										if (it != clients.end()) {
												clients.erase(it);
												return true;
										}
								}
						}
						{
								CommLock connectLock(connectQueueMutex);
								if (connectingClientID == id) {
										connectingClientID = 0;
										return true;
								}
								for (auto it = connectQueue.begin(); it != connectQueue.end(); it++) {
										if (it->first == id) {
												connectQueue.erase(it);
												return true;
										}
								}
						}
						return false;
				}

				bool TCPLink::Send(uint8_t dest_id, uint8_t * tx_data, uint32_t tx_length)
				{
						clientsMutex.Lock();
						auto it = clients.find(dest_id);
						if (it != clients.end()) {
								TcpPtr tcp = it->second;
								clientsMutex.Unlock();
								if (tcp->socket != nullptr) {
										if (tcp->socket->SockSend((const char*)tx_data, tx_length) != 0) {
												std::string msg = "Delete TcpSocket for client with ID ";
												msg += std::to_string(dest_id);
												COMMS_DEBUG(msg.c_str());
												SetSocket(dest_id, nullptr);
										}
								}
								clientsMutex.Lock();
						}
						clientsMutex.Unlock();
						return false;
				}

				bool TCPLink::Recv(uint8_t * rx_data, uint32_t * rx_length)
				{
						RunHandlers();
						clientsMutex.Lock();
						auto it = clients.begin();
						while (it != clients.end()) {
								uint8_t id = it->first;
								auto tcp = it->second;
								it++;
								clientsMutex.Unlock();
								if (tcp->socket != nullptr) {
										packet_data_status_t recvStatus = tcp->socket->SockReceive((const char*)rx_data, MAX_BUFFER_SIZE, *rx_length);
										if (recvStatus == PACKET_SUCCESSFUL) {
												return true;  //TODO: Handle receive errors (should disconnect)
										}
								}
								clientsMutex.Lock();
						}
						clientsMutex.Unlock();
						return false;
				}

				bool TCPLink::RunHandlers()
				{
						if (connectThread == nullptr) {
								connectThread = new CommThread(&TCPLink::ConnectHandler, this);
								acceptThread = new CommThread(&TCPLink::AcceptHandler, this);
								connectThread->Detach();
								acceptThread->Detach();
						}
						return true;
				}

				void TCPLink::AcceptHandler()
				{
						sockaddr_in connectedAddr;
						while (true) {
								CommSocket* socket = local->SockAccept(connectedAddr);
								if (socket != nullptr) {
										bool found = false;
										uint8_t id = AddressToID(connectedAddr.sin_port, connectedAddr.sin_addr, socket, found);
										if (found) {
												COMMS_DEBUG("ACCEPTED!!!\n\n");
												SetSocket(id, socket);
												connectedAddr = sockaddr_in();
										}
										else
										{
												delete socket;
												socket = nullptr;
										}
								}
								clientsMutex.Lock();
								uint32_t size = clients.size();
								clientsMutex.Unlock();
								if (size > 0) {
										std::this_thread::sleep_for(std::chrono::milliseconds(ACCEPT_DELAY_MILLIS));
								}
								else {
										acceptCondVar.Wait();
								}
						}
				}

				void TCPLink::ConnectHandler()
				{
						while (true) {
								connectQueueMutex.Lock();
								if (!connectQueue.empty()) {
										auto clientInfo = connectQueue.front();
										connectQueue.pop_front();
										connectingClientID = clientInfo.first;
										connectQueueMutex.Unlock();
										if (commID < clientInfo.first) {
												Connect(clientInfo.second);
												AddClient(clientInfo);
										}
										else
										{
												AddClient(clientInfo);
												Connect(clientInfo.second);
										}
								}
								else
								{
										connectQueueMutex.Unlock();
										connectCondVar.Wait();
								}
						}
				}

				bool TCPLink::SetSocket(uint8_t id, CommSocket * replacement)
				{
						clientsMutex.Lock();
						clients.at(id) = std::make_shared<TCPHolder>(*clients.at(id), replacement);
						clientsMutex.Unlock();
						return true;
				}

				bool TCPLink::AddClient(std::pair<uint8_t, TcpPtr>& clientInfo)
				{
						clientsMutex.Lock();
						if (connectingClientID != 0) {
								clients.emplace(std::make_pair(clientInfo.first, clientInfo.second));
								connectingClientID = 0;
						}
						clientsMutex.Unlock();
						acceptCondVar.Set();
						return true;
				}

				bool TCPLink::Connect(TcpPtr tcp)
				{
						CommSocket* tcpSocket = CreateTcpSocket();
						if (tcpSocket->SockConnect(tcp->addressInput.c_str(), tcp->portInput) == 0) {
								if (tcp->socket == nullptr) {
										USHORT portInByteOrder = htons(localPort);
										char buffer[PORT_PAYLOAD_SIZE];
										buffer[0] = portInByteOrder & 0xff;
										buffer[1] = (portInByteOrder >> 8) & 0xff;
									 tcpSocket->SockSend(buffer, PORT_PAYLOAD_SIZE);

										uint32_t recvSize;
										char recvBuffer[PORT_REPLY_SIZE];
										for (int i = 0; i < RECV_PORT_REPLY_ATTEMPTS; i++) {
												COMMS_DEBUG("looking_for_port_reply");
												if (tcpSocket->SockReceive(recvBuffer, PORT_REPLY_SIZE, recvSize) == PACKET_SUCCESSFUL) {
														if (recvSize == PORT_REPLY_SIZE) {
																if ((uint8_t)recvBuffer[0] > 0) {
																		tcp->socket = tcpSocket;
																		COMMS_DEBUG("CONNECTED!!!\n\n");
																		return true;
																}
														}
														COMMS_DEBUG("Port handshake failed");
														break;
												}
												std::this_thread::sleep_for(std::chrono::milliseconds(RECV_PORT_REPLY_DELAY_MILLIS));
										}
								}
								else
								{
										COMMS_DEBUG("Connection socket was ignored : not null");
								}
						}
						delete tcpSocket;
						tcpSocket = nullptr;
						return false;
				}

				uint8_t TCPLink::AddressToID(USHORT port, IN_ADDR address, CommSocket* socket, bool & success)
				{
						uint16_t connectedListenPort = 0;
						clientsMutex.Lock();
						for (auto it = clients.begin(); it != clients.end(); it++) {
								uint8_t id = it->first;
								TcpPtr tcp = it->second;
								clientsMutex.Unlock();
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
										if (connectedListenPort == tcp->port) {
												char buffer[PORT_REPLY_SIZE];
												buffer[0] = 0x01;
												socket->SockSend(buffer, PORT_REPLY_SIZE);
												success = true;
												return id;
										}
								}
								clientsMutex.Lock();
						}
						clientsMutex.Unlock();
						char buffer[PORT_REPLY_SIZE];
						buffer[0] = 0x00;
						socket->SockSend(buffer, PORT_REPLY_SIZE);
						success = false;
						return 0;
				}

				TCPHolder::TCPHolder(TCPHolder & copy, CommSocket * replacement)
						:socket(replacement)
				{
						this->addressInput = copy.addressInput;
						this->address = copy.address;
						this->portInput = copy.portInput;
						this->port = copy.port;
				}

				TCPHolder::TCPHolder(uint16_t port, const char * addr, CommSocket * socket)
						:socket(socket)
				{
						addressInput.append(addr);
						portInput = port;
						this->port = htons(port);
						inet_pton(AF_INET, addr, &(address.s_addr));
				}
}
}