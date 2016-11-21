/*
  Represents a client that needs to be pinged to check if still connected.

  Copyright (C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

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
#ifndef __PINGER_H
#define __PINGER_H

#include <ctime>
#include <stdint.h>
#include <CommProto/architecture/os/comm_mutex.h>

namespace comnet {
namespace ping {

using namespace comnet::architecture::os;

typedef int32_t MillisInt;

/**
  Handles the ping of one remote CommNode.
*/
class Pinger
{
public:
		/**
		  The amount of time passed since lastReceiveTime before a pingpacket shoudl be sent.
		*/
		static const MillisInt PING_TIME_MILLIS = 1000;

		/**
	  	The amount of time passed since lastReceiveTime before a connection should be marked as inactive.
		*/
		static const MillisInt CLOSE_TIME_MILLIS = PING_TIME_MILLIS * 2;

		/**
		  Creates a new instance of Pinger given a destID.
		*/
  Pinger(uint8_t destID);

		/**
		  Rests the lastReceiveTime to the current time.
		*/
		void ResetReceiveTime();

		/**
		  Gets the amount of time in seconds before another PingPacket needs to be send,
				if positive, no packet needs to be send, if negative, send the packet.
		*/
		MillisInt GetNextPingTimeMillis();

		/**
		  The id of the remote CommNode associated with this Pinger.
		*/
		uint8_t GetDestID()
		{
				return destID;
		}

		/**
		  Default destructor.
		*/
  ~Pinger();

private:
		/**
		  The id of the remote CommNode associated with this pinger
		*/
		const uint8_t destID;

		/**
		  The last time since a packet was received.
		*/
		clock_t lastReceiveTime;

		/**
		  Prevents lastReceiveTime from being modified and read at the same time.
		*/
		CommMutex receiveTimeMutex;
};
} //namespace ping
} //namespace comnet
#endif //__PINGER_H
