#define _DEBUG
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
#include <stdlib.h>

#include <iostream>
using namespace std;
using namespace Comnet::Serialization;
using namespace Comnet;

int apple(const header_t& gho, const Ping& pl) {
  return pl.num;
}

int main(int c, char** args) {
       
  printf("Hello World!\n");
  Comms newComms1 = Comms(1);
  Comms newComms2 = Comms(2);
  newComms1.initConnection(UDP_LINK, "1337", "127.0.0.1");
  newComms2.initConnection(UDP_LINK, "1338", "127.0.0.1");


  newComms1.addAddress(2, "127.0.0.1", 1338);
  newComms2.addAddress(1, "127.0.0.1", 1337);


	

	newComms2.run();
	newComms1.run();

	uint8_t source;
	uint16_t message;

	while (true)
	{
		newComms1.send(NULL, 2, 1);
	    Sleep(1000);
		newComms2.receive(source, message);
	}
  
  return 0;
}
