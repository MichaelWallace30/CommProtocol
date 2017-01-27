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

#ifndef __CSHARP_PINGER_H
#define __CSHARP_PINGER_H

#include <cstdint>
#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>

namespace Comnet {
  namespace Ping {
#pragma managed
    using namespace System;

    typedef long MillisInt;

    /**
    Handles the pinging of one remote {@link CommNode}.
    */
    public ref class Pinger
    {
    public:
						static Diagnostics::Stopwatch^ START_TIME = Diagnostics::Stopwatch::StartNew();
      
						static MillisInt GetTimeSinceStart() {
								return (MillisInt)START_TIME->ElapsedMilliseconds;
						}
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

						static const int NUM_SYNC_PACKS = 8;
      /**
      The maximum amount times to attempt to send a {@link PingPacket} wihtout
      receiving any response from {@link #destID}.  Once exceeded, the {@link Pinger}
      is considered inactive.
      */
      static const uint8_t MAX_PING_ATTEMPTS = 5;

      /**
      Gets the difference between the current time and the time point provided.
      @param time The time to be compared to the current time.
      @return The amount of milliseconds that have passed since time.
      */
      static MillisInt GetMillisPassed(System::Diagnostics::Stopwatch^ time) {
        return (MillisInt)time->ElapsedMilliseconds;	//amount of time that has passed since lastPingTime
      }

      /**
      Creates a new instance of {@link Pinger}.
      @param destID The nodeID the {@link Pinger} will be sending and
      receiving from.
      @return A new instance of {@link Pinger}.
      */
      Pinger(uint8_t destID);

						Void ResetSyncPackSentTime();

      /**
      Resets {@link #lastPingTime} to the current time.  Sets {@link #pingTime} to
      {@link #PING_RESEND_TIME_MILLIS} because this method is only called after a
      {@link PingPacket} was sent to {@link #destID}.
      */
      Void ResetToResendPingTime();

      /**
      Resets the {@link #lastPingTime} to the current time.
      Sets {@link #pingTime} to {@link #PING_TIME_MILLIS} because calling this method
      means a packet from comms node with {@link #destID} was received.
      */
      Void ResetReceiveTime();

      /**
      Resets the {@link #lastSendTime} to the current time.
      Called whenever a packet is sent to {@link #destID}.
      */
      Void ResetSendTime();

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
        sendTimeMutex->WaitOne();
        MillisInt timePassed = GetMillisPassed(lastSendTime);
        sendTimeMutex->ReleaseMutex();
        return timePassed;
      }

      /**
      Checks whether {@code this} should still be pinged by seeing if {@link #pingAttempts} exceeds {@link #MAX_PING_ATTEMPTS}.
      @return {@code true} when should no longer be pinged, {@code false} when should be pinged.
      */
      Boolean IsInactive() {
        System::Timers::Timer timer;
        pingAttemptsMutex->WaitOne();
        bool result = (pingAttempts > MAX_PING_ATTEMPTS);
        pingAttemptsMutex->ReleaseMutex();
        return result;
      }

						Boolean IsSynced();

						Void SyncTime(int32_t timeOff);

						Void ResetPing(int32_t time);

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
      Diagnostics::Stopwatch^ lastPingTime;

      /**
      Stores the last time a packet was sent to the {@link #destID}.
      */
      Diagnostics::Stopwatch^ lastSendTime;

      /**
      Prevents {@link #lastPingTime} from being modified and read at the same time.
      */
      Threading::Mutex^ pingTimeMutex;

      /**
      Prevents {@link #lastSendTime} from being modified and read at the same time.
      */
      Threading::Mutex^ sendTimeMutex;

      /**
      Prevents {@link #pingAttempts} from being modified and read at the same time.
      */
      Threading::Mutex^ pingAttemptsMutex;

						uint8_t numSyncPacksReceived;

						int32_t timeOffMillis;

						MillisInt syncSendDelay;

						int16_t ping;

						Diagnostics::Stopwatch^ lastSyncPackSentTime;

						Threading::Mutex^ pingMutex;

						Threading::Mutex^ syncMutex;
    };
  } //namespace ping
} //namespace comnet
#endif //__PINGER_H
