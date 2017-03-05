#include <CommProto/network/tcplink.h>
#include <CommProto/debug/comms_debug.h>
#include <iostream>

namespace comnet {

		namespace network {
				TCPLink::~TCPLink()
				{
						free_pointer(acceptThread);
						free_pointer(connectThread);
						free_pointer(local);
				}

				bool TCPLink::InitConnection(const char * port, const char * address, uint32_t baud)
				{
						local = CreateTcpSocket();
						uint16_t portNum = std::atoi(port);
						local->SockListen(address, portNum);
						this->localPort = htons(portNum);
						inet_pton(AF_INET, address, &(this->localIP.s_addr));
						return true;
				}

				bool TCPLink::AddAddress(uint8_t dest_id, const char * address, uint16_t port)
				{
						clientsMutex.Lock();
						auto conInfo = std::make_pair(dest_id, std::make_shared<TCPHolder>(port, address, dest_id));
						conInfo.second->connect = ShouldConnect(conInfo);
						clients.emplace(conInfo);
						clientsMutex.Unlock();
						if (conInfo.second->connect) {
								connectModifyQueueMutex.Lock();
								connectModifyQueue.push(std::make_pair(true, conInfo.second));
								connectModifyQueueMutex.Unlock();
								connectCondVar.Set();
						}
						else
						{
								acceptModifyQueueMutex.Lock();
								acceptModifyQueue.push(std::make_pair(true, conInfo.second));
								acceptModifyQueueMutex.Unlock();
								acceptCondVar.Set();
						}
						return true;
				}

				bool TCPLink::RemoveAddress(uint8_t id)
				{
						CommLock clientLock(clientsMutex);
						auto it = clients.find(id);
						if (it != clients.end()) {
								if (it->second->socket != nullptr) {
										if (it->second->connect) {
												CommLock lock(connectModifyQueueMutex);
												connectModifyQueue.push(std::make_pair(false, it->second));
										}
										else
										{
												CommLock lock(acceptModifyQueueMutex);
												acceptModifyQueue.push(std::make_pair(false, it->second));
										}
								}
								clients.erase(it);
								return true;
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
								acceptModifyQueueMutex.Lock();
								while (!acceptModifyQueue.empty()) {
										auto clientPair = acceptModifyQueue.front();
										acceptModifyQueue.pop();
										if (clientPair.first) {
												acceptList.push_back(clientPair.second);
										}
										else
										{
												acceptList.remove(clientPair.second);
										}
								}
								acceptModifyQueueMutex.Unlock();
								if (!acceptList.empty()) {
										CommSocket* socket = local->SockAccept(connectedAddr);
										if (socket != nullptr) {
												bool found = false;
												uint8_t id = AddressToID(connectedAddr.sin_port, connectedAddr.sin_addr, socket, found);
												if (found) {
														std::cout << "\n" << (int)id << " ACCEPTED" << std::endl;
														SetSocket(id, socket);
														connectedAddr = sockaddr_in();
												}
												else
												{
														delete socket;
														socket = nullptr;
												}
										}
										std::this_thread::sleep_for(std::chrono::milliseconds(ACCEPT_DELAY_MILLIS));
								}
								else 
								{
										acceptCondVar.Wait();
								}
						}
				}

				void TCPLink::ConnectHandler()
				{
						while (true) {
								connectModifyQueueMutex.Lock();
								while (!connectModifyQueue.empty()) {
										auto clientPair =connectModifyQueue.front();
										connectModifyQueue.pop();
										if (clientPair.first) {
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
												if (Connect(*it)) {
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
										connectCondVar.Wait();
								}
						}
				}

				bool TCPLink::SetSocket(uint8_t id, CommSocket * replacement)
				{
						CommLock lock(clientsMutex);
						auto it = clients.find(id);
						if (it != clients.end()) {
								it->second = std::make_shared<TCPHolder>(*clients.at(id), replacement);
								if (replacement == nullptr) {
										if (it->second->connect) {
												connectModifyQueueMutex.Lock();
												connectModifyQueue.push(std::make_pair(true, it->second));
												connectModifyQueueMutex.Unlock();
												connectCondVar.Set();
										}
										else
										{
												acceptModifyQueueMutex.Lock();
												acceptModifyQueue.push(std::make_pair(true, it->second));
												acceptModifyQueueMutex.Unlock();
												acceptCondVar.Set();
										}
								}
						}
						return true;
				}

				bool TCPLink::AddClient(std::pair<uint8_t, TcpPtr>& clientInfo)
				{
						clientsMutex.Lock();
						clients.emplace(std::make_pair(clientInfo.first, clientInfo.second));
						clientsMutex.Unlock();
						acceptCondVar.Set();
						return true;
				}

				bool TCPLink::Connect(TcpPtr tcp)
				{
						CommSocket* tcpSocket = CreateTcpSocket();
						if (tcpSocket->SockConnect(tcp->addressInput.c_str(), tcp->portInput) == 0) {
								char buffer[PORT_PAYLOAD_SIZE];
								buffer[0] = localPort & 0xff;
								buffer[1] = (localPort >> 8) & 0xff;
								tcpSocket->SockSend(buffer, PORT_PAYLOAD_SIZE);

								uint32_t recvSize;
								char recvBuffer[PORT_REPLY_SIZE];
								for (int i = 0; i < RECV_PORT_REPLY_ATTEMPTS; i++) {
										std::cout << "\n" << "looking for port reply" << std::endl;
										if (tcpSocket->SockReceive(recvBuffer, PORT_REPLY_SIZE, recvSize) == PACKET_SUCCESSFUL) {
												if (recvSize == PORT_REPLY_SIZE) {
														if ((uint8_t)recvBuffer[0] > 0) {
																std::cout << "\n" << "CONNECTED" << std::endl;
																SetSocket(tcp->destID, tcpSocket);
																return true;
														}
												}
												COMMS_DEBUG("Port handshake failed");
												break;
										}
										std::this_thread::sleep_for(std::chrono::milliseconds(RECV_PORT_REPLY_DELAY_MILLIS));
								}
						}
						tcpSocket->SockClose();
						delete tcpSocket;
						tcpSocket = nullptr;
						return false;
				}

				bool TCPLink::ShouldConnect(std::pair<uint8_t, TcpPtr> conInfo)
				{
						if (conInfo.second->port < localPort) {
								return true;
						}
						else if (conInfo.second->port > localPort)
						{
								return false;
						}
						else
						{
								if (conInfo.second->address.s_addr < localIP.s_addr) {
										return true;
								}
								else
								{
										return false;
								}
						}
				}

				uint8_t TCPLink::AddressToID(USHORT port, IN_ADDR address, CommSocket* socket, bool & success)
				{
						uint16_t connectedListenPort = 0;
						for (auto it = acceptList.begin(); it != acceptList.end(); it++) {
								TcpPtr tcp = *it;
								if (address.s_addr == tcp->address.s_addr)
								{
										if (connectedListenPort == 0) {
												char buffer[PORT_PAYLOAD_SIZE];
												uint32_t size = 0;
												for (int i = 0; i < RECV_PORT_ATTEMPTS; i++) {
														std::cout << "\n" << "looking for port" << std::endl;
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
												acceptList.erase(it);
												return tcp->destID;
										}
								}
						}
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
						this->connect = copy.connect;
						this->destID = copy.destID;
				}

				TCPHolder::TCPHolder(uint16_t port, const char * addr, uint8_t destID, CommSocket * socket)
						:socket(socket),destID(destID)
				{
						addressInput.append(addr);
						portInput = port;
						this->port = htons(port);
						inet_pton(AF_INET, addr, &(address.s_addr));
				}

				TCPHolder::~TCPHolder() {
						if (socket != nullptr) {
								if (this->connect) {
										socket->SockClose();
								}
								delete socket;
								socket = nullptr;
						}
				}
}
}