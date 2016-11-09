#define __COMMS_DEBUG_FATAL_EXIT
#include <CommProto/comms.h>
#include <CommProto/abstractpacket.h>
#include <CommProto/callback.h>
#include <vector>
#include <string>
#include <iostream>
/*
#Note
CommProtocol Library requires libxbee3 to execute copy the file to your bin folder (ie .exe)
-Windows {build dir}\libxbee3\(debug or release)\libxbee3.dll
-Linux: {build dir}\libxbee3\libxbee3.o
*/

/*
#Note
This examples requires that you have two COM ports on you computer that
are connected to one another.  Most computers don't have RS 232 ports anymore.
Check https://docs.google.com/document/d/1WCYMmVNjQgmBkN0y59NzXw2j078tIpOrO-Y1PAn8LAk/edit?usp=sharing for alternatives.
*/

//First COM port
static const char* PORT1 = "COM5";

//Second COM port
static const char* PORT2 = "COM6";

//Amount of milliseconds between sends
static const int SEND_SLEEP_MILLIS = 4000;

//Simple AbstractPacket that Packs and UnPacks a string.
class Ping : INHERITS_ABSPACKET {
public:
		Ping()
				: CHAIN_ABSPACKET(Ping)
		{

		}

		void Pack(OBJECTSTREAM& outStream) override
		{
				outStream << data;
		}

		void Unpack(OBJECTSTREAM& inStream) override
		{
				inStream >> data;
		}

		void SetData(const std::string& data)
		{
				this->data = data;
		}

		std::string GetData() const
		{
				return data;
		}

		ABSPACKET* Create()
		{
				return new Ping();
		}

private:
		std::string data;
};

//Callback for Ping
error_t PingCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {
		std::cout << "=::RECEIVED PACKET::=" << std::endl;
		std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
		std::cout << "Message: " << std::endl;
		std::cout << "Packet contains: " << packet.GetData() << std::endl;
		return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}

int main()
{
		std::condition_variable cond;
		comnet::Comms comm1(1);
		if (comm1.LoadKey("NGCP Project 2016"))
		{
				std::cout << "Key loaded successfully" << std::endl;
		}
		comnet::Comms comm2(2);
		if (comm2.LoadKey("NGCP Project 2016"))
		{
				std::cout << "Key loaded successfully" << std::endl;
		}
		comnet::architecture::os::CommMutex mut;
		comnet::architecture::os::CommLock commlock(mut);
		comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);		//Stop this thread for 1 second

		//Open the serial port to the value of PORT1
		if (!comm1.InitConnection(SERIAL_LINK, PORT1, NULL, 9600))
		{
				std::cerr << "Unable to initialize port " << PORT1 << std::endl;
		}

		//Open the serial port the value of PORT2
		if (!comm2.InitConnection(SERIAL_LINK, PORT2, NULL, 9600))
		{
				std::cerr << "Unable to initialize port " << PORT2 << std::endl;
		}

		comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
		comm2.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
		Ping bing;
		bing.SetData("Hans Solo Dies :)");
		comm1.Run();
		comm2.Run();
		while (true)
		{
				std::cout << "Sleeping..." << std::endl;
				comm1.Send(bing, 2);
				std::this_thread::sleep_for(std::chrono::milliseconds(SEND_SLEEP_MILLIS));
		}
		std::cin.ignore();
		return 0;
}
