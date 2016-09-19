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
#include <CommsWrapper.h>
#include <CommProto/Comms.h>

#pragma managed
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace Comnet;

#define _DEBUG 1

	//globabl test out of not in scope of class
	error_t storeFunction(const header_t& header, AbstractPacket& ping) {
		printf("function called\n");
		return 1;
	}

	CommsWrapper::CommsWrapper(uint8_t platformID)
	{
		unmangedComms = new Comms(platformID);
		ping = new Ping(100);
		unmangedComms->linkCallback(new Ping(0), new Callback(storeFunction));
	}
	CommsWrapper::~CommsWrapper()
	{
		if (unmangedComms != NULL)
		{
			delete unmangedComms;
			unmangedComms = NULL;
		}
	}


	bool CommsWrapper::initConnection(transport_protocol_tC connectionType, String^ port, String^ address, uint32_t baudrate)
	{
		//convert String^ to char*
		IntPtr ptrToNativeStringPort = Marshal::StringToHGlobalAnsi(port);
		char* nativePort = static_cast<char*>(ptrToNativeStringPort.ToPointer());
		//convert String^ to char*
		IntPtr ptrToNativeStringAddress = Marshal::StringToHGlobalAnsi(address);
		char* nativeAddress = static_cast<char*>(ptrToNativeStringAddress.ToPointer());
		//call unmanged initConnection

		return unmangedComms->initConnection(static_cast<transport_protocol_t>(connectionType), nativePort, nativeAddress, baudrate);

	}

	bool CommsWrapper::addAddress(uint8_t destID, String^ address, uint16_t port)
	{
		IntPtr ptrToNativeStringAddress = Marshal::StringToHGlobalAnsi(address);
		char* nativeAddress = static_cast<char*>(ptrToNativeStringAddress.ToPointer());
		return unmangedComms->addAddress(destID, nativeAddress, port);

	}

	bool CommsWrapper::removeAddress(uint8_t destID)
	{
		return unmangedComms->removeAddress(destID);
	}

	bool CommsWrapper::send(uint8_t destID)
	{
		//testing only
		printf("sending\n");
		return unmangedComms->send(ping, destID);
	}

	AbstractPacket* CommsWrapper::receive(uint8_t&  sourceID)
	{
		return unmangedComms->receive(sourceID);
	}

	bool CommsWrapper::linkCallback(ABSPacket^ packet, const Callback* callback)
	{
		return unmangedComms->linkCallback(packet->getAbstractPacket(), callback);
	}
	int32_t CommsWrapper::run()
	{
		return unmangedComms->run();
	}

	int32_t CommsWrapper::pause()
	{
		return unmangedComms->pause();
	}

	int32_t CommsWrapper::stop()
	{
		return unmangedComms->stop();
	}



