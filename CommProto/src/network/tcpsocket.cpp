/*
	TCP/IP connections.

	Copyright (C) 2016  Michael Wallace, Mario Garcia.

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
#include <CommProto/network/commsocket.h>
#include <CommProto/architecture/connection/socket-config.h>
#include <CommProto/debug/log.h>
#include <CommProto/architecture/os/os_threads.h>

#include <cstdlib>
#include <ctime>

#if COMMPROTO_TARGET_OS != COMMMPROTO_OS_WINDOWS
#include <netinet/tcp.h>
#endif

#define MAX_TICK 5

namespace comnet {
	namespace network {
		/**
			TcpSocket is a class intended on creating a socket connection to a host
			computer. It is created via abstractly, without the need for the user to
			know what on earth this class is. Instead, user creates this tcp connection through
			an interface. This class can Create a listening server, or a client from which to make
			connections with.
		*/
		class COMM_EXPORT TcpSocket : public CommSocket {
		public:

			/**
			Initialize the sockaddr struct with tcp protocol signatures.
			*/
			static void InitializeSockAddr(const char* address, PORT port, struct sockaddr_in* s) {
				s->sin_family = AF_INET;
				s->sin_port = htons(port);
				inet_pton(AF_INET, address, &(s->sin_addr));
			}

			/**
				Constructor method for initializing this socket.
			 */
			TcpSocket() {
				bool du = false;
				initializeSockAPI(du);
				_socket.socket_status = SOCKET_CLOSED;
				_socket.socket = INVALID_SOCKET;
				_socket.port = -1;
				_socket.id = -1;
			}
			/**
				Default destructor
			*/
			~TcpSocket() {
			}

			/**
				Connection function, from which a socket is created for the purpose of sending data
				to the server and Back. This will Create a client socket syncronously.
			*/
			int32_t SockConnect(const char* address, PORT port) override
			{
				int32_t error = -1;
				if (_socket.socket != INVALID_SOCKET) {
					return error;
				}

				_socket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (_socket.socket == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_ERROR, "Invalid socket...");
				}
				else {
					if (!SetNonBlocking(_socket.socket, true)) {
						debug::Log::Message(debug::LOG_ERROR, "Failed to set socket to non blocking mode...");
						_socket.socket_status = SOCKET_FAILED;
					}
					else {
						struct sockaddr_in host = { 0 };
						InitializeSockAddr(address, port, &host);

						_socket.socket_status = SOCKET_CONNECTING;
						int32_t connResult = connect(_socket.socket, (struct sockaddr*)&host, sizeof(host));
						if (connResult != INVALID_SOCKET ||
							(GET_LAST_ERROR != EWOULDBLOCK &&
								GET_LAST_ERROR != EINPROGRESS))
						{
							debug::Log::Message(debug::LOG_ERROR, "Cannot initiate connection");
							_socket.socket_status = SOCKET_FAILED;
						}
						else {
							int errLen = sizeof(error);
							timeval timeOut = { 0 };
							timeOut.tv_sec = MAX_TICK;
							timeOut.tv_usec = 0;
							fd_set fdset;
							FD_ZERO(&fdset);
							FD_SET(_socket.socket, &fdset);
							error = select(0, NULL, &fdset, NULL, &timeOut);
							if (error == SOCKET_ERROR) {
								debug::Log::Message(debug::LOG_ERROR, "FAILED");
							}
							if (getsockopt(_socket.socket, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t*)&errLen) != 0) {
								debug::Log::Message(debug::LOG_ERROR, "error in getsockopt");
								_socket.socket_status = SOCKET_FAILED;
							}
							if (error == 0) {
								debug::Log::Message(debug::LOG_NOTIFY, "Successful connection!");
								_socket.socket_status = SOCKET_CONNECTED;
							}
						}
					}
				}

				return error;
			}

			/**
				Sends information over the connecting socket, in this case, the socket that
				is initialized in this tcp connection.
				@param buffer
				@param len
				@param address
				@param port
			*/
			int32_t SockSend(const char* buffer,
				uint32_t len,
				const char* address,
				uint32_t port) override
			{
				int32_t error = -1;
				if (_socket.socket == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_ERROR, "socket is not connected...\n");
					return error;
				}

				if (send(_socket.socket, buffer, len, 0) < 0) {
					LOG_PRINTF(debug::LOG_ERROR, "Socket failed to send with error: %d\n", GET_LAST_ERROR);
					error = -1;
				}
				else {
					debug::Log::Message(debug::LOG_NOTIFY, "Socket succeeded in sending data.");
					error = 0;
				}
				return error;
			}

			/**
				Recieve information over the connecting socket, in this case, the socket that is
				initialized in this tcp connection.
			 */
			packet_data_status_t SockReceive(const char* buffer,
				uint32_t len,
				uint32_t& size,
				const char* address,
				uint32_t port) override
			{
				packet_data_status_t packetStatus = PACKET_NO_DATA;
				if (_socket.socket == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_ERROR, "Socket is not connected...\n");
					return packetStatus;
				}

				int32_t retVal = recv(_socket.socket, (char*)buffer, len, 0);
				if (retVal <= 0)
				{
					//debug::Log::Message(debug::LOG_DEBUG, "Unsuccessful packet recieved...\n");
					packetStatus = PACKET_MORE_DATA;
				}
				else {
					size = retVal;
					//debug::Log::Message(debug::LOG_DEBUG, "Successful packet recieved...\n");
					packetStatus = PACKET_SUCCESSFUL;
				}

				return packetStatus;
			}

			/**
				Connect to socket asyncronously, without having to wait for the time tick.
				Instead, the connection is handled via a thread.
			*/
			int32_t SockAsyncConnect(const char* address, uint32_t port) override
			{
				return 0;
			}

			/**
				Set the listening server for this connection, in this case, the tcpsocket
				will initialize a server socket.
			*/
			int32_t SockListen(const char* address, uint32_t port) override
			{
				int32_t error = -1;
				if (_socket.socket_status != SOCKET_CLOSED) {
					debug::Log::Message(debug::LOG_ERROR, "Socket is not closed before listening...");
					return error;
				}

				_socket.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (_socket.socket == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_ERROR, "Unsuccessful socket creation...");
				}
				else {
					struct sockaddr_in host;
					InitializeSockAddr(address, port, &host);
					if (bind(_socket.socket, (struct sockaddr*)&host, sizeof(host)) < 0) {
						debug::Log::Message(debug::LOG_ERROR, "Failed to bind socket...");
					}
					else {
						if (listen(_socket.socket, SOMAXCONN) < 0) {
							debug::Log::Message(debug::LOG_ERROR, "Failed in listen function...");
						}
						else {
							if (!SetNonBlocking(_socket.socket, true)) {
								debug::Log::Message(debug::LOG_ERROR, "Failed to set non blocking socket...");
							}
							else {
								debug::Log::Message(debug::LOG_NOTIFY, "Socket is successfully listening...");
								error = 0;
								_socket.socket_status = SOCKET_LISTENING;
								_socket.port = port;
							}
						}
					}
				}
				return error;
			}

			int32_t SockListen(uint32_t port) override
			{
				return 0;
			}

			/**
				Accept a connection from any client. If a connection was successfully accepted,
				A new TcpSocket will be created, along with initializing the client socket that was accepted into
				the new TcpSocket, and returned. Otherwise, NULL will be returned.
			*/
			CommSocket* SockAccept() override
			{
				if (_socket.socket_status != SOCKET_LISTENING) {
					debug::Log::Message(debug::LOG_WARNING, "socket is not listening...");
					return NULL;
				}

				CommSocket* tcpSock = NULL;
				SOCKET s = accept(_socket.socket, NULL, NULL);
				if (s == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_NOTE, "Failed to accept...");
				}
				else {
					debug::Log::Message(debug::LOG_NOTIFY, "Successful accept...");
					SetTcpNoDelay(s, true);
					tcpSock = new TcpSocket(s);
				}
				return tcpSock;
			}

			CommSocket* SockAccept(sockaddr_in& connectedAddress) override
			{
				if (_socket.socket_status != SOCKET_LISTENING) {
					debug::Log::Message(debug::LOG_ERROR, "socket is not listening...");
					return NULL;
				}

				CommSocket* tcpSock = NULL;
#ifdef _WIN32
				int32_t addrSize = sizeof(connectedAddress);
#else
				uint32_t addrSize = sizeof(connectedAddress);
#endif
				SOCKET s = accept(_socket.socket, (sockaddr*)(&connectedAddress), &addrSize);
				if (s == INVALID_SOCKET) {
					debug::Log::Message(debug::LOG_NOTE, "Failed to accept...");
				}
				else {
					debug::Log::Message(debug::LOG_NOTIFY, "Successful accept...");
					SetTcpNoDelay(s, true);
					tcpSock = new TcpSocket(s);
				}
				return tcpSock;
			}

			/**
				Close the socket.
			*/
			void SockClose() override
			{
				closeSocket(_socket.socket);
				_socket.socket = INVALID_SOCKET;
				_socket.socket_status = SOCKET_CLOSED;

				debug::Log::Message(debug::LOG_NOTIFY, "Socket has closed.");
			}

		protected:
			/**
				Set this socket to asyncronous io, that way it does not Stop the thread just to wait for
				a function to complete its call.
			*/
			bool SetNonBlocking(SOCKET sock, bool on) {
#ifdef _WIN32
				return ioctlsocket(sock, FIONBIO, (u_long*)&on) == 0;
#else
				int flags = fcntl(sock, F_GETFL, 0);
				return fcntl(sock, F_SETFL, on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK)) == 0;
#endif
			}

			/**
				Set the tcp connection to have no delay (disables Nagle's Algorithm) in betweeen reading messages.
				The packet is recieved instantly, otherwise an error is quickly returned. This is similar to
				using UDP conneciton.
			*/
			bool SetTcpNoDelay(SOCKET sock, bool on) {
				return setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(on)) == 0;
			}
		private:
			/**
				Private constructor, only used to Create a new socket from accept().
			*/
			TcpSocket(SOCKET socket) {
				bool du = false;
				_socket.socket_status = SOCKET_CONNECTED;
				_socket.socket = socket;
			}

			/**
				The connected client socket (if used connect()) or server socket (if used listen()).
			*/
			socket_t _socket;
		};

		/**
			Creates a tcp socket for CommSocket.
		*/
		CommSocket* CreateTcpSocket() {
			return new TcpSocket();
		}
	} // Network namespace
} // Comnet namespace