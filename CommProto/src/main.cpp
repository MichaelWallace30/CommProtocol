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
#include <CommProto/pkg/PacketManager.h>
#include <stdlib.h>

#include <iostream>
using namespace std;
using namespace Comnet::Serialization;
using namespace Comnet;
using namespace Comnet::Pkg;

int apple(const header_t& gho, const Ping& pl) {
  cout << "I got an apple" << endl;
  return pl.num;
}

int main(int c, char** args) {
  Ping d(12);
  Ping a(122);
  Pong ponger('1');
	header_t head;
  Callback call;
	call.setCallbackListener((callback_t)apple);
	cout << "Call: " << call.callFunction(head, d) << endl;

  PacketManager packageManager;
  // Sample test of storing Ping Callback pair. Should be done this way.
  packageManager.insert(new Ping(0), new Callback((callback_t)apple));
  Callback* callResult = packageManager.get(a);
  if (!callResult) {
    cout << "NULL" << endl;
    cin.ignore();
  }
  int value = callResult->callFunction(head, a);
  cout << "This is the result... " << value << endl;
  cin.ignore();

	ObjectStream newObjectStream = ObjectStream();

	double value1A = 64.2;
	int value2A = 12;
	string_t strData = (char*)"Hello Slim!!!";
	uint8_t valueA = 1;

	std::wstring wStrA = L"Hello World Wide????";

	newObjectStream << valueA << strData << wStrA << value2A << value1A;

	double value1B;
	int value2B;
	uint8_t valueB;
	std::wstring wStrB;
	string_t strDataB = (string_t)malloc(25);

	//extra varible
	int extraVar;

	newObjectStream >> value1B >> value2B >> wStrB >> strDataB >> valueB;

    cout<<(int)valueB<<endl;
    cout<<value1B<<endl;
    cout << value2B <<endl;
    cout << strDataB <<endl;
    wcout << wStrB << endl;

    //prints error does not crash
    newObjectStream >> extraVar;
    Comms comms(1);
    comms.linkCallback(new Ping(1), new Callback((callback_t)apple));
    /*
  printf("Hello World!\n");
  Comms newComms1 = Comms(1);
  Comms newComms2 = Comms(2);
  newComms1.initConnection(UDP_LINK, "1337", "127.0.0.1");
  newComms2.initConnection(UDP_LINK, "1338", "127.0.0.1");


  newComms1.addAddress(2, "127.0.0.1", 1338);
  newComms2.addAddress(1, "127.0.0.1", 1337);


	

	newComms2.run();
	newComms1.run();

	Ping ping = Ping(1);
	uint8_t source;
	uint16_t message;

	while (true)
	{
		newComms1.send(&ping, 2, 1);
	    Sleep(1000);
		newComms2.receive(source, message);
	}
  */
  return 0;
}
