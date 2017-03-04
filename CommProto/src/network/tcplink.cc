#include <CommProto/network/tcplink.h>
#include <iostream>

namespace comnet {

		namespace network {
				TCPLink::~TCPLink()
				{
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
						connectQueue.push(std::make_pair(dest_id, std::make_shared<TCPHolder>(port, address)));
						connectQueueMutex.Unlock();
						connectCondVar.Set();
						return true;
				}

				bool TCPLink::RemoveAddress(uint8_t dest_id)
				{
						return false;
				}

				bool TCPLink::Send(uint8_t dest_id, uint8_t * tx_data, uint32_t tx_length)
				{
						clientsMutex.Lock();
						auto it = clients.find(dest_id);
						if (it != clients.end()) {
								if (it->second->socket != nullptr) {
										it->second->socket->SockSend((const char*)tx_data, tx_length);
								}
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
								auto tcp = it->second;
								it++;
								clientsMutex.Unlock();
								if (tcp->socket != nullptr) {
										if (tcp->socket->SockReceive((const char*)rx_data, MAX_BUFFER_SIZE, *rx_length) == PACKET_SUCCESSFUL) {
												
												std::cout << "RECEIVED" << std::endl; //temp
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
												SetSocket(id, socket);
												connectedAddr = sockaddr_in();  //necessary?
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
										std::this_thread::sleep_for(std::chrono::milliseconds(ACCEPT_DELAY));
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
										connectQueue.pop();
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
						clients.emplace(std::make_pair(clientInfo.first, clientInfo.second));
						clientsMutex.Unlock();
						acceptCondVar.Set();
						return true;
				}

				bool TCPLink::Connect(TcpPtr tcp)
				{
						CommSocket* tcpSocket = CreateTcpSocket();
						if (tcpSocket->SockConnect(tcp->addressInput.c_str(), tcp->portInput) == 0) {
								tcp->socket = tcpSocket;
								std::cout << localPort << std::endl;
								USHORT portInByteOrder = htons(localPort);
								char buffer[2];
								buffer[0] = portInByteOrder & 0xff;
								buffer[1] = (portInByteOrder >> 8) & 0xff;
								tcp->socket->SockSend(buffer, 2);
								return true;
						}
						else {
								delete tcpSocket;
								tcpSocket = nullptr;
								return false;
						}
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
												char buffer[2];
												uint32_t size = 0;
												for (int i = 0; i < 10; i++) {
														if (socket->SockReceive(buffer, 2, size)) {
																if (size == 2) {
																		connectedListenPort = buffer[0] & 0xff;
																		connectedListenPort |= buffer[1] << 8;

																		std::cout << "Connected port: " << ntohs(connectedListenPort) << std::endl;
																}
																break;
														}
														std::this_thread::sleep_for(std::chrono::milliseconds(250));
												}
										}
										if (connectedListenPort == tcp->port) {
												success = true;
												return id;
										}
								}
								clientsMutex.Lock();
						}
						clientsMutex.Unlock();
						success = false;
						return 0;
				}

				bool TCPLink::RemoveClient(uint8_t id)
				{
						return false;
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