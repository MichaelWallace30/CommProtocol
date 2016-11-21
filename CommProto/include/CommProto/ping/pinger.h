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

#include <chrono>
#include <stdint.h>
#include <CommProto/architecture/os/comm_mutex.h>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::time_point <std::chrono::steady_clock> TimePoint;
typedef std::chrono::duration<float> fsec;

namespace comnet {
namespace ping {

using namespace comnet::architecture::os;

typedef long MillisInt;

/**
  Handles the ping of one remote CommNode.
*/
class Pinger
{
public:
		/**
		  The amount of time passed since lastReceiveTime before a pingpacket shoudl be sent.
		*/
		static const MillisInt PING_TIME_MILLIS = 5000;

		static const MillisInt PING_RESEND_TIME_MILLIS = 2000;

		/**
	  	The amount of time passed since lastReceiveTime before a connection should be marked as inactive.
		*/
		static const MillisInt CLOSE_TIME_MILLIS = PING_TIME_MILLIS * 2;

		/**
		  Creates a new instance of Pinger given a destID.
		*/
  Pinger(uint8_t destID);

		/**
		  Resets lastPingTime to current time.  Sets pingTime to 
				PING_RESEND_TIME_MILLIS because this method is only called after a ping packet was sent.
		*/
		void ResetToResendPingTime();

		/**
		  Resets the lastReceiveTime and lastPingTime to the current time.
				Sets pingTime to PING_TIME_MILLIS because calling this method means a packet from this destID was received.
		*/
		void ResetReceiveTime();

		/**
		  Gets the amount of time in seconds before another PingPacket needs to be send,
				if positive, no packet needs to be send, if negative, send the packet.
		*/
		MillisInt GetNextPingTimeMillis();

		/**
		  The destinationID of the comm node associated with this Pinger
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
		  The amount of milliseconds after lastPingTime to send another ping packet.
		*/
		MillisInt pingTime;

		/**
		  The destination ID of the comm node associated with this Pinger.
		*/
		uint8_t destID;

		/**
		  The last time since a packet was received.
		*/
		TimePoint lastReceiveTime;

		/**
		  Will be set to current time whenever a packet has been received or a ping has been sent.
				Used by the GetNextPingTimeMilliseconds() to help determine when to send anotehr ping packet.
		*/
		TimePoint lastPingTime;

		/**
		  Prevents lastReceiveTime from being modified and read at the same time.
		*/
		CommMutex receiveTimeMutex;
};
} //namespace ping
} //namespace comnet
#endif //__PINGER_H
