/*
Link for UDP connections.

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

#ifndef __TCP_LINK_H
#define __TCP_LINK_H

#include <CommProto/network/commslink.h>
#include <CommProto/network/commsocket.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/architecture/os/comm_condvar.h>
#include <CommProto/architecture/connection/socket-config.h>
#include <memory>
#include <unordered_map>
#include <list>
#include <queue>
#include <atomic>

namespace comnet {
	namespace network {
		using namespace comnet::architecture::os;

		struct TCPHolder {
			TCPHolder(TCPHolder& copy, CommSocket* replacement);
			TCPHolder(uint16_t port, const char* addr, uint8_t destID, CommSocket* socket = nullptr);

			~TCPHolder();

			bool connect;
			USHORT port;
			IN_ADDR address;
			CommSocket* socket;
			uint16_t portInput;
			std::string addressInput;
			uint8_t destID;
		};

		typedef std::shared_ptr<TCPHolder> TcpPtr;

		class COMM_EXPORT TCPLink : public CommsLink {
		public:
			static const int ACCEPT_DELAY_MILLIS;
			static const int RECV_PORT_ATTEMPTS;
			static const int RECV_PORT_DELAY_MILLIS;
			static const int RECV_PORT_REPLY_ATTEMPTS;
			static const int RECV_PORT_REPLY_DELAY_MILLIS;

			static const int CON_DELAY_MILLIS;
			static const uint8_t PORT_PAYLOAD_SIZE;
			static const uint8_t PORT_REPLY_SIZE;

			static const uint8_t ADD_QUEUE_EVENT;
			static const uint8_t REMOVE_QUEUE_EVENT;

			static const uint8_t PACKET_SIZE_NUM_BYTES;

			typedef uint32_t PacketSizeInt;

			TCPLink()
				:connectThread(nullptr)
			{
				protocol = TCP_LINK;
			}

			~TCPLink();

			bool InitConnection(const char* port = NULL, const char* address = NULL, uint32_t baudrate = 0) override;

			bool AddAddress(uint8_t dest_id, const char* address = NULL, uint16_t port = 0) override;

			bool RemoveAddress(uint8_t dest_id) override;

			bool Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length) override;

			bool Recv(uint8_t* rx_data, uint32_t* rx_length) override;

			void DigestCommand(const char* cmd) override { }

		protected:
			bool RunHandlers();
			void AcceptHandler();
			void ConnectHandler();
			bool SetSocket(uint8_t id, CommSocket* replacement);
			bool Connect(TcpPtr conInfo);
			bool ShouldConnect(TcpPtr conInfo);
			uint8_t AddressToID(USHORT port, IN_ADDR address, CommSocket* socket, bool& success);

			CommSocket* local;
			IN_ADDR localIP;
			USHORT localPort;

			std::list<TcpPtr> connectList;
			std::queue<std::pair<bool, TcpPtr>> connectModifyQueue;
			CommMutex connectModifyQueueMutex;
			CommThread* connectThread;
			CommConditionVariable connectCondVar;

			std::list <TcpPtr> acceptList;
			std::queue <std::pair<bool, TcpPtr>> acceptModifyQueue;
			CommMutex acceptModifyQueueMutex;
			CommThread* acceptThread;
			CommConditionVariable acceptCondVar;

			std::unordered_map<uint8_t, TcpPtr> clients;
			CommMutex clientsMutex;
		};
	}
}
#endif