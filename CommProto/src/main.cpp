#define _DEBUG 1
#include <stdio.h>
#include <CommProto/Comms.h>
#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/hash/NgcpHash.h>
#include <CommProto/console/Console.h>
#include <CommProto/network/CommSocket.h>
#include <CommProto/serialization/Marshal.h>
#include <CommProto/serialization/ObjectStream.h>
#include <CommProto/Callback.h>
#include <CommProto/Packets.h>
#include <CommProto/pkg/PacketManager.h>
#include <CommProto/tools/data_structures/LinkedQueue.h>
#include <stdlib.h>
#include <CommProto/tools/data_structures/DoubleLinkedList.h>
#include <CommProto/tools/Comparator.h>

#include <typeinfo>
#include <iostream>
using namespace std;
using namespace Comnet::Tools::DataStructures;
using namespace Comnet::Serialization;
using namespace Comnet;
using namespace Comnet::Pkg;


int apple(const header_t& gho, AbstractPacket& pl) {
  cout << "I got an apple " << AbstractPacket::getValue<Ping>(pl).num << endl;
  cout << "Test: " << AbstractPacket::getValue<Ping>(pl).test << endl;
  return CALLBACK_SUCCESS;
}

int testingFunction(const header_t& header, Ping& ping) {
  cout << "I am a new testing function" << endl;
  return AbstractPacket::getValue<Ping>(ping).num;
}

// Class Ping Comparator.
class PingComparator : public Tools::Comparator<Ping*> {
public:
  PingComparator() { }
  
  int32_t compare(Ping* const& ping1, Ping* const& ping2) {
    if (ping1->num < ping2->num) {
      return -1;
    } else {
      return 1;
    }
  }

  bool equal(Ping* const& ping1, Ping* const& ping2) {
    return ping1->num == ping2->num;
  }
};

static DoubleLinkedList<Ping*, PingComparator> pinger;

class Storage {
public:
  static error_t storeFunction(const header_t& header, AbstractPacket& ping) {
    cout << "Recieved ping" << endl;
    pinger.insert((Ping *) &(AbstractPacket::getValue<Ping>(ping)));
    return 0;
  }
};

int main(int c, char** args) {
  Ping d(12);
  Ping a(122);
  Pong aaaa('f');
  AbstractPacket& pa = aaaa;
  AbstractPacket::getValue<Ping>(pa).num;
  Pong ponger('1');
	header_t head;
  Callback call;
	call.setCallbackListener((callback_t)testingFunction);
	cout << "Call: " << call.callFunction(head, d) << endl;

  PacketManager packageManager;
  cout << "inserting..." << endl;
  // Sample test of storing Ping Callback pair. Should be done this way.
  //packageManager.insert(new Ping(0), NULL);
  cout << "finished inserting" << endl;
  int value;
  cout << "Yep" << endl;
  Callback* callResult = packageManager.get(a);
  cout << "finished" << endl;
  if (callResult) {
    value = callResult->callFunction(head, a);
    cout << "This is the result... " << value << endl;
    cin.ignore();
  }

  packageManager.insert(new Ping(1), new Callback([] (const header_t& header, AbstractPacket& packet) {
    Ping& ping = AbstractPacket::getValue<Ping>(packet);
    cout << "This stuff now works: " << ping.num << endl;
    return -1;
  }));
  delete callResult; callResult = NULL;
  callResult = packageManager.get(a);
  value = callResult->callFunction(head, a);
  cin.ignore();

	ObjectStream newObjectStream = ObjectStream();

	double value1A = 64.2;
	int value2A = 12;
	std::string strData = "Hello Slim!!!";
	uint8_t valueA = 1;

	std::wstring wStrA = L"Hello World Wide????";

	newObjectStream << valueA << strData << wStrA << value2A << value1A;

	double value1B;
	int value2B;
	uint8_t valueB;
	std::wstring wStrB;
	std::string strDataB;

	//extra varible
	int extraVar;

	newObjectStream >> value1B >> value2B >> wStrB >> strDataB >> valueB;

    cout<< (int)valueB << endl;
    cout<< value1B << endl;
    cout << value2B <<endl;
    cout << strDataB <<endl;
    wcout << wStrB << endl;

    // prints error does not crash
    COMMS_DEBUG("\nTesting error runtime.\n");
    newObjectStream >> extraVar;
    COMMS_DEBUG("\nTesting Abstract Object CommNode\n");
    cin.ignore();
    
  printf("Hello World!\n");
  Comms comm1(1);
  Comms comm2(2);
  CommNode& newComms1 = comm1;
  CommNode& newComms2 = comm2;
  // Can now replace queues.
  newComms2.replaceReceiveQueue(new LinkedQueue<AbstractPacket*>());
  bool success = newComms2.linkCallback(new Ping(0), new Callback(Storage::storeFunction));
  if (success) {
    COMMS_DEBUG("\nSuccessfully added!\n");
  }
  
  COMMS_DEBUG("Adding callback\n");
  newComms1.linkCallback(new Ping(0), new Callback(Storage::storeFunction));
  COMMS_DEBUG("Callback added!\n");
  success = newComms1.initConnection(UDP_LINK, "1338", "127.0.0.1");
  cout << "newComms1 init: " << success << endl;
  success = newComms2.initConnection(UDP_LINK, "1337", "127.0.0.1");
  cout << "newComms2 init: " << success << endl;
  cin.ignore();

  success = newComms1.addAddress(2,"127.0.0.1", 1337);
  cout << "newComms1 add: " << success << endl;
  success = newComms2.addAddress(1, "127.0.0.1", 1338);
  cout << "newComms2 add: " << success << endl;

	
  COMMS_DEBUG("RUNNING!");
  cin.ignore();
  cout << newComms2.getNodeId() << endl;
	newComms2.run();
	newComms1.run();
	cout << newComms1.getNodeId() << endl;
	Ping ping(100);
	uint8_t source;
	uint16_t message;

	while (true)
	{
	  newComms1.send(&ping, 2);
    if (!pinger.isEmpty()) {
      Ping* pinging = pinger.front();
      pinger.removeAt(0);
      // Must remove the pinging packet manually if you attempt to grab it.
      free_pointer(pinging);
    }
    Sleep(1000);
    // This is not needed unless the user hasn't specified a callback for the packet, and
    // they need to get the packet manually.
		//newComms2.receive(source, message);
	}

	COMMS_DEBUG("FINISHED!!");
  
  return 0;
}
