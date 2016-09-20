#include <iostream>

#include <CommsWrapper.h>
#include <ABSPacket.h>
#include <CallBack.h>


using namespace Comnet;
using namespace std;


public ref class Testing : public ABSPacket {
public:
  Testing() : ABSPacket("Testing") { }

  void Pack(ObjectStreamWrapper^ obj) override {
  
  }

  void Unpack(ObjectStreamWrapper^ obj) override {
    
  }

  ABSPacket^ Create() override {
    return gcnew Testing();
  }
private:
};

Int32 test(HeaderWrapper^ header, ABSPacket^ packet) {
  return -1;
}

int main() {
  CommsWrapper comm1(1);
  CommsWrapper comm2(2);


  comm1.linkCallback(gcnew Testing(), gcnew CallBack(gcnew Testing(), gcnew CommFunct(test)));

  comm1.initConnection(TransportProtocol::UDP_LINK, gcnew String("1337"), gcnew String("127.0.0.1"), 12);
  comm2.initConnection(TransportProtocol::UDP_LINK, gcnew String("1338"), gcnew String("127.0.0.1"), 0);

  comm1.addAddress(2, gcnew String("127.0.0.1"), 1338);
  comm2.addAddress(1, gcnew String("127.0.0.1"), 1337);

  while(1) {
    
  }
  cin.ignore();
  return 0;
}