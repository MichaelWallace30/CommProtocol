/*
Packet for sending and receiving pings

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

#ifndef __CSHARP_PING_MANAGER_H
#define __CSHARP_PING_MANAGER_H

#include <gcroot.h>
#include <list>
#include <unordered_map>
#include <vcclr.h>
#using <mscorlib.dll>
#using <System.dll>
#include <Header.h>
#include <CallBack.h>
#include <ping/Pinger.h>

namespace Comnet {
  ref class Comms;
  namespace Ping {
    ref class PingPacket;
    using namespace System::Collections::Generic;
#pragma managed

    Int32 PingCallback(Header^ header, ABSPacket^ absPacket, CommNode^ node);

    public ref class PingManager {
    public:
      typedef gcroot<Pinger^> Pinger_Ptr;
      static const MillisInt EMPTY_SLEEP_TIME_MILLIS = Pinger::PING_TIME_MILLIS;

      PingManager(Comms^ owner);

      Void LinkPingCallback();

      Boolean Run();

      Boolean IsActive(uint8_t destID);

      Void ResetPingTime(uint8_t destID);

      Void ResetSendTime(uint8_t destID);

      Void HandlePingUpdate();

      Void AddPinger(uint8_t destID);

      Void RemovePinger(uint8_t destID);

      Boolean CanPong(uint8_t destID);

      Void SendPingPacket(uint8_t destID);

      Void Stop();

      ~PingManager();

      !PingManager();

    private:
      Void TransferToActivePingers(std::list<Pinger_Ptr>::iterator it);

      Void TransferToInactivePingers(std::list<Pinger_Ptr>::iterator it);

      std::list<Pinger_Ptr>* activePingers;

      std::list<Pinger_Ptr>* inactivePingers;

      std::unordered_map<uint8_t, std::list<Pinger_Ptr>::iterator>* destPingerMap;

      Threading::Mutex^ inactivePingersMutex;

      Threading::Mutex^ activePingersMutex;

      Threading::Mutex^ destPingerMapMutex;

      Threading::Thread^ pingSendThread;

      Comms^ owner;  //May prevent deletion of Comms

      bool running;

      Threading::Mutex^ runningMutex;

      PingPacket^ pingPacket;
    };
  }
}

#endif