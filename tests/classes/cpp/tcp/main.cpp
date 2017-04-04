/*
  Sample test with using the CommProtocol library.  

  Copyright (C) 2016  Michael Wallace, Kartik Soni, Mario Garcia, Alex Craig.

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
#include <CommProto/commproto.h>

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
  //Disables Pinging to make reading output easier
  comnet::constate::ConnectionStateManager::ConStateEnabled = false;

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
    << comm2.InitConnection(TCP_LINK, "1338", "127.0.0.1")
    << std::endl;
  std::cout << "Connected to address: "
    << std::boolalpha
    << comm2.AddAddress(1, "127.0.0.1", 1337)
    << std::endl;
  // CommNode 2 init and add Connection.
  std::cout << "Init connection succeeded: "
    << std::boolalpha
    << comm1.InitConnection(TCP_LINK, "1337", "127.0.0.1")
    << std::endl;
  std::cout << "Connected to address: "
    << std::boolalpha
    << comm1.AddAddress(2, "127.0.0.1", 1338)
    << std::endl;
  // CommNode Callback linking.
  comm1.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));
  comm2.LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

  // Allow client to suppress or unsuppress messages handled by the CommProtocol Library.
  comnet::debug::Log::Suppress(comnet::debug::LOG_NOTIFY);
  comnet::debug::Log::Suppress(comnet::debug::LOG_WARNING);
  comnet::debug::Log::Suppress(comnet::debug::LOG_NOTE);

  // Test packet.
  Ping bing("I like cats. MEW :3. this is a test...");
  // NOTE(All): Be sure to run the nodes! If not, the threads won't execute!
  comm1.Run();
		//std::this_thread::sleep_for(std::chrono::milliseconds(20000));
  comm2.Run();

  // Loop. To exit, Click the red button on the top left (Windows Visual Studio) OR 
  // CNTRL+C (Linux). 
  while (true) {
    std::cout << "Sleeping..." << std::endl;
    //comm1 will be sending the packet.
    comm1.Send(bing, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cin.ignore();
  return 0;
}
