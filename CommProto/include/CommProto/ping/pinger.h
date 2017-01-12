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
typedef std::chrono::high_resolution_clock::time_point TimePoint;// std::chrono::time_point <std::chrono::steady_clock> TimePoint;
typedef std::chrono::duration<float> fsec;

namespace comnet {
namespace ping {

using namespace comnet::architecture::os;

typedef long MillisInt;

/**
  Handles the pinging of one remote {@link CommNode}.
*/
class Pinger
{
public:
  /**
    The amount of that must pass since the last time a packet
    was received from the {@link #destID} before
    a {@link PingPacket} is sent.
  */
  static const MillisInt PING_TIME_MILLIS = 7500;

  /**
    The amount of milliseconds that must pass since {@link #lastPingTime} before
    attempting to send another {@link PingPacket} after no response from the {@link #destID}.
  */
  static const MillisInt PING_RESEND_TIME_MILLIS = 2500;

  /**
    The amount of milliseconds that must pass since {@link #lastSendTime} before
    a Pong reply will be send when receiving a {@link PingPacket}.
  */
  static const MillisInt PONG_TIME_MILLIS = 1500;

		static const MillisInt SYNC_PACK_SEND_MILLIS = 2000;

		static const MillisInt CHECK_OFF_MILLIS = 50;

		static const int NUM_SYNC_PACKS = 3;

  /**
    The maximum amount times to attempt to send a {@link PingPacket} wihtout
    receiving any response from {@link #destID}.  Once exceeded, the {@link Pinger}
    is considered inactive.
  */
  static const uint8_t MAX_PING_ATTEMPTS = 5;

		static const TimePoint START_TIME;
  /**
    Gets the current time.
    @return The current time.
  */
  static TimePoint GetNow() {
    return Time::now();    
  }

  /**
    Gets the difference between the current time and the time point provided.
    @param time The time to be compared to the current time.
    @return The amount of milliseconds that have passed since time.
  */
  static MillisInt GetMillisPassed(TimePoint time) {
    fsec fs = GetNow() - time;	//amount of time that has passed since lastPingTime
    ms millis = std::chrono::duration_cast<ms>(fs);		//converting time to milliseconds
    return static_cast<comnet::ping::MillisInt>(millis.count());
  }

		static MillisInt GetTimeSinceStart() {
				return GetMillisPassed(START_TIME);
		}

  /**
    Used for copy and swap idiom.
    @param first {@link Pinger} to be copied to.
    @param second {@link Pinger} that is about to be destroyed.
  */
  static void swap(Pinger& first, Pinger& second) {
    std::swap(first.destID, second.destID);
    std::swap(first.pingTime, second.pingTime);
    std::swap(first.lastPingTime, second.lastSendTime);
    std::swap(first.lastSendTime, second.lastSendTime);
    std::swap(first.pingAttempts, second.pingAttempts);
  }


  /**
    Creates a new instance of {@link Pinger}.
    @param destID The nodeID the {@link Pinger} will be sending and
    receiving from.
    @return A new instance of {@link Pinger}.
  */
  Pinger(uint8_t destID);

  /**
    Copies a {@link Pinger}. Not thread-safe.
    @param other The {@link Pinger} to be copied.
    @return A new instance of {@link Pinger}.
  */
  Pinger(Pinger& other) 
  {
    destID = other.destID;
    pingTime = other.pingTime;
    pingAttempts = other.pingAttempts;
    lastPingTime = other.lastPingTime;
    lastSendTime = other.lastSendTime;
  }

  /**
    Move constructor that copies the {@link Pinger} passed into it.
    Not thread-safe.
    @param mover The {@link Pinger} to be copied.
    @return A new instance of {@link Pinger}.
  */
  Pinger(Pinger&& mover)
  {
    swap(*this, mover);
  }

  /**
    Uses the copy and swap idiom to copy the passed in {@link Pinger} to {@code this}.
    @param other {@link Pinger} to be copied.
    @return A new instance of {@link Pinger}.
  */
  Pinger& operator=(Pinger other) {
    swap(*this, other);
    return *this;
  }

		/**
		*/
		void ResetSyncPackSentTime()
		{
				CommLock lock(syncMutex);
				lastSyncPackSentTime = GetNow();
		}

  /**
    Resets {@link #lastPingTime} to the current time.  Sets {@link #pingTime} to 
    {@link #PING_RESEND_TIME_MILLIS} because this method is only called after a 
    {@link PingPacket} was sent to {@link #destID}.
  */
  void ResetToResendPingTime();

  /**
    Resets the {@link #lastPingTime} to the current time.
    Sets {@link #pingTime} to {@link #PING_TIME_MILLIS} because calling this method 
    means a packet from comms node with {@link #destID} was received.
  */
  void ResetReceiveTime();

  /**
    Resets the {@link #lastSendTime} to the current time.
    Called whenever a packet is sent to {@link #destID}.
  */
  void ResetSendTime();

  /**
    Accessor for the {@link #destID} of the comm_node associated with {@code this}.
    @return A copy of {@link #destID}.
  */
  uint8_t GetDestID()
  {
    return destID;
  }

  /**
    Calculates the amount of milliseconds since {@link #lastSendTime}.
    @return Milliseconds since {@link #lastSendTime}.
  */
  MillisInt GetSendTimePassed()
  {
    CommLock lock(sendTimeMutex);
    return GetMillisPassed(lastSendTime);
  }

  /**
   Checks whether {@code this} should still be pinged by seeing if {@link #pingAttempts} exceeds {@link #MAX_PING_ATTEMPTS}.
   @return {@code true} when should no longer be pinged, {@code false} when should be pinged.
  */
  bool IsInactive() {
    CommLock lock(pingAttemptsMutex);
    return (pingAttempts > MAX_PING_ATTEMPTS);
  }

		bool IsSynced();

		void SyncTime(int32_t timeOff);

		void ResetPing(int32_t time);

  /**
  Gets the amount of time in milliseconds before another {@link PingPacket} needs to be sent,
  if positive, no packet needs to be send, if negative, send the packet.
  @return The amount of milliseconds before another {@link PingPacket} should be sent.
  */
  MillisInt GetNextPingTimeMillis();

		MillisInt GetNextSyncTimeMillis();

		int16_t GetPing()
		{
				return ping;
		}

  /**
    Default destructor.
  */
  ~Pinger();

private:
  /**
    Stores how many times a {@link PingPacket} has been sent to the {@link #destID}
    without receiving any packet.  Reset to 0 by {@link #ResetReceiveTime} and 
    incremented by {@link #ResetToResetPingTime}.
  */
  uint8_t pingAttempts;

  /**
    The amount of milliseconds after lastPingTime to send another ping packet.
  */
  MillisInt pingTime;

  /**
    The {@link CommNode#node_id} associated with {@code this}. Used to
    check if times should be reset when recieving and sending to a
    {@link CommNode#node_id}.
  */
  uint8_t destID;

  /**
    Will be set to current time whenever a packet has been received or a ping has been sent.
    Used by the {@link #GetNextPingTimeMilliseconds()} to determine when to send another {@link PingPacket}.
  */
  TimePoint lastPingTime;

  /**
    Stores the last time a packet was sent to the {@link #destID}.
  */
  TimePoint lastSendTime;

  /**
    Prevents {@link #lastPingTime} from being modified and read at the same time.
  */
  CommMutex pingTimeMutex;

  /**
    Prevents {@link #lastSendTime} from being modified and read at the same time.
  */
  CommMutex sendTimeMutex;

  /**
    Prevents {@link #pingAttempts} from being modified and read at the same time.
  */
  CommMutex pingAttemptsMutex;

		uint8_t numSyncPackReceieved;

		int32_t timeOffMillis;

		int16_t ping;

		TimePoint lastSyncPackSentTime;

		CommMutex pingMutex;

		CommMutex syncMutex;
};
} //namespace ping
} //namespace comnet
#endif //__PINGER_H
