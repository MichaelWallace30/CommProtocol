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
						char* portChar = (char*)(void*)Marshal::StringToHGlobalAnsi(port);
						char* addressChar = nullptr;
						if (address != nullptr) {
								addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
						}
						local = comnet::network::CreateTcpSocket();
						uint16_t portNum = std::atoi(portChar);
						if (local->SockListen(addressChar, portNum) == 0) {
								this->localPort = htons(portNum);
								inet_pton(AF_INET, addressChar, &(this->localIP->s_addr));
								return true;
						}
						COMMS_DEBUG("Could not listen on the specified port and address");
						return false;
				}

				Boolean TCPLink::AddAddress(uint8_t destID, String ^ address, uint16_t port)
				{
						char* addressChar = (char*)(void*)Marshal::StringToHGlobalAnsi(address);
						QueueEntry^ entry = gcnew QueueEntry();
						entry->add = true;
						entry->tcp = gcnew TCPHolder(port, address, destID, nullptr);
						entry->tcp->connect = ShouldConnect(entry->tcp);
						if (!clients->TryAdd(destID, entry->tcp)) {
								COMMS_DEBUG("TCPLink: Could not add client (already exists?)");
								return false;
						}
						if (entry->tcp->connect) {
								connectModifyQueue->Enqueue(entry);
								connectRE->Set();
						}
						else
						{
								acceptModifyQueue->Enqueue(entry);
								acceptRE->Set();
						}
						return true;
				}

				Boolean TCPLink::RemoveAddress(uint8_t destID)
				{
						TCPHolder^ tcp;
						if (clients->TryGetValue(destID, tcp)) {
								if (tcp->socket != nullptr) {
										QueueEntry^ entry = gcnew QueueEntry();
										entry->add = false;
										entry->tcp = tcp;
										if (tcp->connect) {
												connectModifyQueue->Enqueue(entry);
										}
										else
										{
												acceptModifyQueue->Enqueue(entry);
										}
								}
								if (!clients->TryRemove(destID, tcp)) {
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
										if (tcp->socket->SockSend((const char*)txData, txLength) != 0) {
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
						RunHandlers();
						auto iter = clients->GetEnumerator();
						while (iter->MoveNext()) {
								TCPHolder^ tcp = iter->Current.Value;
								if (tcp->socket != nullptr) {
										uint32_t recvSize = 0;
										comnet::network::packet_data_status_t recvStatus = tcp->socket->SockReceive((const char*)rxData, MAX_BUFFER_SIZE, recvSize);
										if (recvStatus == comnet::network::PACKET_SUCCESSFUL) {
												rxLength = recvSize;
												return true;
										}
								}
						}
						return false;
				}

				Boolean TCPLink::RunHandlers() {
						if (connectThread == nullptr) {
								connectThread = gcnew Thread(gcnew ThreadStart(this, &TCPLink::ConnectHandler));
								connectThread->Start();
								acceptThread = gcnew Thread(gcnew ThreadStart(this, &TCPLink::AcceptHandler));
								acceptThread->Start();
								return true;
						}
						return false;
				}

				Void TCPLink::AcceptHandler()
				{
						sockaddr_in connectedAddr;
						while (true) {
								QueueEntry^ entry;
								while (acceptModifyQueue->TryDequeue(entry)) {
										if (entry->add) {
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
										comnet::network::CommSocket* socket = local->SockAccept(connectedAddr);
										if (socket != nullptr) {
												bool found = false;
												uint8_t id = AddressToID(connectedAddr.sin_port, connectedAddr.sin_addr, socket, found);
												if (found) {
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
										acceptRE->WaitOne();
								}
						}
				}

				Void TCPLink::ConnectHandler() {
						while (true) {
								QueueEntry^ entry;
								while (connectModifyQueue->TryDequeue(entry)) {
										if (entry->add) {
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
												LinkedListNode<TCPHolder^>^ next = it->Next;
												if (Connect(it->Value)) {
														connectList->Remove(it);
												}
												it = next;
										}
										Sleep(CON_DELAY_MILLIS);
								}
								else
								{
										connectRE->WaitOne();
								}
						}
				}

				Boolean TCPLink::SetSocket(uint8_t id, comnet::network::CommSocket* replacement) {
						TCPHolder^ tcp;
						if (clients->TryGetValue(id, tcp))
						{
								TCPHolder^ newHolder = gcnew TCPHolder(tcp, replacement);
								clients[id] = newHolder;
								if (replacement == nullptr) {
										QueueEntry^ entry = gcnew QueueEntry();
										entry->add = true;
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

				Boolean TCPLink::Connect(TCPHolder^ tcp)
				{
						comnet::network::CommSocket* tcpSocket = comnet::network::CreateTcpSocket();
						char* addrChar = (char*)(void*)Marshal::StringToHGlobalAnsi(tcp->addressInput);
						if (tcpSocket->SockConnect(addrChar, tcp->portInput) == 0)
						{
								std::vector <char> sendBuffer;
							 sendBuffer.resize(PORT_PAYLOAD_SIZE);
								sendBuffer[0] = localPort & 0xff;
								sendBuffer[1] = (localPort >> 8) & 0xff;
								if (tcpSocket->SockSend(sendBuffer.data(), PORT_PAYLOAD_SIZE) != 0)
								{
										COMMS_DEBUG("Error sending port handshake");
								}
								else
								{
										uint32_t recvSize;
										std::vector <char> recvBuffer;
										recvBuffer.resize(PORT_REPLY_SIZE);
										for (int i = 0; i < RECV_PORT_REPLY_ATTEMPTS; i++) {
												if (tcpSocket->SockReceive(recvBuffer.data(), PORT_REPLY_SIZE, recvSize) == comnet::network::PACKET_SUCCESSFUL) {
														if (recvSize == PORT_REPLY_SIZE) {
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
						tcpSocket->SockClose();
						delete tcpSocket;
						tcpSocket = nullptr;
						return false;
				}

				Boolean TCPLink::ShouldConnect(TCPHolder^ tcp)
				{
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

				uint8_t TCPLink::AddressToID(USHORT port, IN_ADDR address, comnet::network::CommSocket* socket, bool% success)
				{
						uint16_t connectedListenPort = 0;
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
										if (connectedListenPort == tcp->port) {
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
												acceptList->Remove(it);
												return tcp->destID;
										}
								}
								it = next;
						}
						std::vector <char> sendBuffer;
						sendBuffer.resize(PORT_REPLY_SIZE);
						sendBuffer[0] = 0x00;
						socket->SockSend(sendBuffer.data(), PORT_REPLY_SIZE);
						success = false;
						return 0;
				}

				TCPHolder::TCPHolder(TCPHolder^ copy, comnet::network::CommSocket* replacement)
						:socket(replacement), addressInput(copy->addressInput),
						address(copy->address), portInput(copy->portInput),
						port(copy->port), connect(copy->connect),
						destID(copy->destID)
				{

				}

				TCPHolder::TCPHolder(uint16_t port, String^ addr, uint8_t destID, comnet::network::CommSocket* socket)
						:socket(socket), destID(destID), portInput(port), addressInput(addr)
				{
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