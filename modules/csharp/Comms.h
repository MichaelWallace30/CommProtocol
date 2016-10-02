/*
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
#ifndef __CCOMMS_H
#define __CCOMMS_H

#include <CommNode.h>
#include <network/CommsLink.h>
#include <vcclr.h>
#using <mscorlib.dll>


namespace Comnet {

#pragma managed
using namespace System;
using namespace Comnet::Network;


// CComms node.
public ref class Comms : public CommNode {
public:
  Comms(UInt32 id);
  ~Comms();
  
  Boolean InitConnection(TransportProtocol connType, 
            String^ port, String^ addr, UInt32 baudrate) override;

  Boolean AddAddress(UInt16 destId, String^ addr, UInt16 port) override;
  Boolean RemoveAddress(UInt16 destId) override;
  Boolean Send(ABSPacket^ packet, Byte destId) override;
  ABSPacket^ Receive(Byte% sourceId) override;

  Void Run() override;
  Void Pause() override;
  Void Stop() override;

private:
  Network::CommsLink^ connLayer;

  System::Threading::Thread^ sendThr;
  System::Threading::Thread^ recvThr;

  System::Threading::Mutex^ sendMut;
  System::Threading::Mutex^ recvMut;

  Void commHelperSend();
  Void commHelperRecv();
};
}
#endif // __CCOMMS_H