#define _DEBUG 1
#include <stdio.h>
#include <CommProto/comms.h>
#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/hash/ngcphash.h>
#include <CommProto/console/console.h>
#include <CommProto/network/commsocket.h>
#include <CommProto/serialization/marshal.h>
#include <CommProto/serialization/objectstream.h>
#include <CommProto/callback.h>
#include <CommProto/packets.h>
#include <CommProto/pkg/packetmanager.h>
#include <CommProto/tools/data_structures/linked_queue.h>
#include <stdlib.h>
#include <CommProto/tools/data_structures/double_linked_list.h>
#include <CommProto/tools/comparator.h>

#include <typeinfo>
#include <iostream>
using namespace std;
using namespace comnet::tools::datastructures;
using namespace comnet::serialization;
using namespace comnet;
using namespace comnet::pkg;


error_t pingCall(const Header& header, AbstractPacket& ping) {
  return 0;
}

int main(int c, char** args) {
  //Comms comm1(1);
  Comms comm2(2);
  //CommNode& newComms1 = comm1;
  CommNode& newComms2 = comm2;
  // Can now replace queues.
  //newComms2.ReplaceReceiveQueue(new LinkedQueue<AbstractPacket*>());
  bool success = newComms2.LinkCallback(new Ping(), new Callback(pingCall));
  if (success) {
    COMMS_DEBUG("\nSuccessfully added!\n");
  }
  
  COMMS_DEBUG("Adding callback\n");
  //newComms1.LinkCallback(new Ping(0), new Callback(Storage::storeFunction));
  COMMS_DEBUG("Callback added!\n");
  //success = newComms1.InitConnection(UDP_LINK, "1338", "127.0.0.1");
  cout << "newComms1 init: " << success << endl;
  success = newComms2.InitConnection(UDP_LINK, "1338", "127.0.0.1");
  cout << "newComms2 init: " << success << endl;
  cin.ignore();

  //success = newComms1.AddAddress(2,"127.0.0.1", 1337);
  cout << "newComms1 add: " << success << endl;
  success = newComms2.AddAddress(1, "127.0.0.1", 1337);
  cout << "newComms2 add: " << success << endl;

	
  COMMS_DEBUG("RUNNING!");
  cin.ignore();
  cout << newComms2.GetNodeId() << endl;
	newComms2.Run();
	//newComms1.Run();
	//cout << newComms1.GetNodeId() << endl;
	Ping ping("I am a cat.");
	uint8_t source;
	uint16_t message;

	while (true)
	{
	  newComms2.Send(&ping, 1);
    Sleep(1000);
    // This is not needed unless the user hasn't specified a callback for the packet, and
    // they need to Get the packet manually.
		//newComms2.Receive(source, message);
	}

	COMMS_DEBUG("FINISHED!!");
  
  return 0;
}
