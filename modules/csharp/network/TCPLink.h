/*
C++/CLI Wrapper for TCPLink

Copyright (C) 2016  Michael Wallace, Mario Garcia.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __CSHARP_TCP_LINK_H
#define __CSHARP_TCP_LINK_H
#include <network/CommsLink.h>
#include <CommProto/network/commsocket.h>
#include <CommProto/architecture/connection/socket-config.h>
#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>

namespace Comnet {
	namespace Network {
#pragma managed

		using namespace System::Collections::Generic;
		using namespace System::Collections::Concurrent;
		using namespace System::Threading;

		public ref class TCPHolder {
		public:
			TCPHolder(TCPHolder^ copy, comnet::network::CommSocket* replacement);
			TCPHolder(uint16_t port, String^ address, uint8_t destID, comnet::network::CommSocket* socket);

			!TCPHolder();
			~TCPHolder();

			Boolean connect;
			USHORT port;
			IN_ADDR* address;
			comnet::network::CommSocket* socket;
			uint16_t portInput;
			String^ addressInput;
			uint8_t destID;
		};

		private ref class QueueEntry {
		public:
			uint8_t qEvent;
			TCPHolder^ tcp;
		};

		public ref class TCPLink : public CommsLink {
		public:
			static const int ACCEPT_DELAY_MILLIS = 2000;
			static const int RECV_PORT_ATTEMPTS = 10;
			static const int RECV_PORT_DELAY_MILLIS = 500;
			static const int RECV_PORT_REPLY_ATTEMPTS = 20;
			static const int RECV_PORT_REPLY_DELAY_MILLIS = 500;

			static const int CON_DELAY_MILLIS = 2000;
			static const uint8_t PORT_PAYLOAD_SIZE = 2;
			static const uint8_t PORT_REPLY_SIZE = 1;

			static const uint8_t ADD_QUEUE_EVENT = 1;
			static const uint8_t REMOVE_QUEUE_EVENT = 2;
			
			static const uint8_t PACKET_SIZE_NUM_BYTES = 4;

			typedef uint32_t PacketSizeInt;

			TCPLink();
			~TCPLink();
			!TCPLink();

			Boolean InitConnection(String^ port, String^ address, uint32_t baudrate) override;

			/**
			Overloaded  initConnection
			*/
			Boolean InitConnection(String^ port, String^ address) {
				return InitConnection(port, address, 0);
			}
			/**
			Adds Address & port to destID value of array of aviable connections
			@returns false if connection is already connected
			*/
			Boolean AddAddress(uint8_t destID, String^ address, uint16_t port) override;
			/**
			Sets connection to not available
			@returns false is no connection is found
			*/
			Boolean RemoveAddress(uint8_t destID) override;
			/**
			Sends txData using its length of bytes through the destID connection which
			is establish through add address.

			@return false if no proper connection is establish
			*/
			Boolean Send(uint8_t destID, uint8_t* txData, uint32_t txLength) override;
			/**
			Sets recieved data to rxData and sets the length of the data to rxLength
			@returns false if not aviable connection or no data is recieved
			*/
			Boolean Recv(uint8_t* rxData, UInt32% rxLength) override;

		private:
			Boolean RunHandlers();
			Void AcceptHandler();
			Void ConnectHandler();
			Boolean SetSocket(uint8_t id, comnet::network::CommSocket* socket);
			Boolean Connect(TCPHolder^ tcp);
			Boolean ShouldConnect(TCPHolder^ entry);
			uint8_t AddressToID(USHORT port, IN_ADDR address, comnet::network::CommSocket* socket, bool% success);

			comnet::network::CommSocket* local;
			IN_ADDR* localIP;
			USHORT localPort;

			LinkedList<TCPHolder^>^ connectList;
			ConcurrentQueue<QueueEntry^>^ connectModifyQueue;
			Thread^ connectThread;
			AutoResetEvent^ connectRE;

			LinkedList<TCPHolder^>^ acceptList;
			ConcurrentQueue<QueueEntry^>^ acceptModifyQueue;
			Thread^ acceptThread;
			AutoResetEvent^ acceptRE;

			ConcurrentDictionary <uint8_t, TCPHolder^>^ clients;
		};
	}
}

#endif