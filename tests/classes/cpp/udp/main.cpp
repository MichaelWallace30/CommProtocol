#include <CommProto/architecture/os/comm_mutex.h>
#include <CommProto/comms.h>
#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_thread.h>
#include <iostream>
#include <CommProto/debug/log.h>
/*
#Note 
CommProtocol Library requires libxbee3 to execute copy the file to your bin folder (ie .exe)
-Windows {build dir}\libxbee3\(debug or release)\libxbee3.dll
-Linux: {build dir}\libxbee3\libxbee3.o
*/

class Cat {
};
// Ping testing.
class Ping : INHERITS_ABSPACKET {
public:
		Ping() : CHAIN_ABSPACKET(Ping) { }
		Ping(std::string cc) 
		: CHAIN_ABSPACKET(Ping)
		, cat(cc) { }

		void Pack(REF_OBJECTSTREAM obj) {
				obj << cat;  // add cat
		}

		void Unpack(REF_OBJECTSTREAM obj) {
				obj >> cat;
		}
		
		ABSPACKET* Create() {
				return new Ping();
		}

		const std::string& GetCat() const { return cat; }
private:

		std::string cat;
};


// Callback function that we will be using to link to Ping packet.
error_t PingCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {
		std::cout << "=::RECEIVED PACKET::=" << std::endl;
		std::cout << std::endl << "Source node: " << (int32_t)header.source_id << std::endl;
		std::cout << "Message: " << std::endl;
		std::cout << "Packet contains: " << packet.GetCat() << std::endl;
		return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}


int main(int c, char** args) {
		std::condition_variable cond;
		std::cout << sizeof(comnet::Header) << std::endl;
		// CommNode 1
		comnet::Comms comm1(1);
		comm1.LoadKey("NGCP project 2016");
		int numClients = 0;
		while (true)
		{
				std::cout << "Enter number of clients: ";
				std::cin >> numClients;
				if (numClients <= 0 || numClients >= 255)
				{
						std::cerr << "Invalid number of clients" << std::endl;
				}
				else
				{
						break;
				}
		}
		// CommNode 
		std::vector <comnet::Comms*> comms;
		for (int i = 0; i < numClients; i++)
		{
				comms.push_back(new comnet::Comms(i + 2));
				comms.at(i)->LoadKey("NGCP project 2016");
		}
		comnet::architecture::os::CommMutex mut;
		comnet::architecture::os::CommLock commlock(mut);
		// This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
		comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000); 

		// Allow client to suppress or unsuppress messages handled by the CommProtocol Library.
		comnet::debug::Log::Suppress(comnet::debug::LOG_NOTIFY);
		comnet::debug::Log::Suppress(comnet::debug::LOG_WARNING);
		comnet::debug::Log::Suppress(comnet::debug::LOG_NOTE);

		// ComNode 1 init and add Connection.
		std::cout << "Init connection succeeded: " 
												<< std::boolalpha
												<< comm1.InitConnection(UDP_LINK, "1338", "127.0.0.1")
												<< std::endl;

		// CommNode Callback linking.
		comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
		
		bool clientInitSuccess = true;
		for (int i = 0; i < comms.size(); i++)
		{
				if (comms.at(i)->InitConnection(UDP_LINK, std::to_string(1339 + i).c_str(), "127.0.0.1") &&
						comms.at(i)->AddAddress(1, "127.0.0.1", 1338) &&
						comm1.AddAddress(i + 2, "127.0.0.1", 1339 + i) &&
						comms.at(i)->LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback)))
				{
						comms.at(i)->Run();
				}
				else
				{
						clientInitSuccess = false;
				}
		}
		if (!clientInitSuccess)
		{
				std::cerr << "Failed to add address" << std::endl;
		}
		else
		{
				std::cout << "Addresses added successfully" << std::endl;
		}

		// Test packet.
		Ping bing("I like cats. MEW :3. this is a test...");
		// NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
		comm1.Run();

		// Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
		// CNTRL+C (Linux). 
		while (true) {
				//std::cout << "Sleeping..." << std::endl;
				//comm1 will be sending the packet.
				//comm1.Send(bing, 2);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		std::cin.ignore();
		return 0;
}
