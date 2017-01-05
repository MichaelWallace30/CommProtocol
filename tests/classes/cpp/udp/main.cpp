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
  std::cout << std::boolalpha << comm1.LoadKey("NGCP project 2016");
  // CommNode 2
  comnet::Comms comm2(2);
  std::cout << std::boolalpha << comm2.LoadKey("NGCP project 2016");
  comnet::architecture::os::CommMutex mut;
  comnet::architecture::os::CommLock commlock(mut);
  // This will cause the thread to wait for a few milliseconds, causing any other thread to wait.
  comnet::architecture::os::WaitForMilliseconds(commlock, cond, 1000);

  // ComNode 1 init and add Connection.
  std::cout << "Init connection succeeded: "
    << std::boolalpha
    << comm2.InitConnection(UDP_LINK, "1338", "127.0.0.1")
    << std::endl;
  std::cout << "Connected to address: "
    << std::boolalpha
    << comm2.AddAddress(1, "127.0.0.1", 1337)
    << std::endl;
  // CommNode 2 init and add Connection.
  std::cout << "Init connection succeeded: "
    << std::boolalpha
    << comm1.InitConnection(UDP_LINK, "1337", "127.0.0.1")
    << std::endl;
  std::cout << "Connected to address: "
    << std::boolalpha
    << comm1.AddAddress(2, "127.0.0.1", 1338)
    << std::endl;
  // CommNode Callback linking.
		comm1.AddPacket(new Ping());
		comm2.AddPacket(new Ping());
  //comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
  //comm2.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

  // Allow client to suppress or unsuppress messages handled by the CommProtocol Library.
  comnet::debug::Log::Suppress(comnet::debug::LOG_NOTIFY);
  comnet::debug::Log::Suppress(comnet::debug::LOG_WARNING);
  comnet::debug::Log::Suppress(comnet::debug::LOG_NOTE);

  // Test packet.
  Ping bing("I like cats. MEW :3. this is a test...");
		Ping bing2("I like dogs.");
  // NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
  comm1.Run();
  comm2.Run();

  // Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
  // CNTRL+C (Linux). 
		comm1.Send(bing, 2);
		comm1.Send(bing2, 2);
		while (true) {
				uint8_t receiveID;
				ABSPACKET* absPack = (ABSPACKET*)comm2.Receive(receiveID);
				std::cout << "RUN" << std::endl;
				if (absPack != nullptr && absPack->GetId() == bing.GetId()) {
						std::cout << ((Ping*)absPack)->GetCat() << std::endl;
				}
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cin.ignore();
  return 0;
}
