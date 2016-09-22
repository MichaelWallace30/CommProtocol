#include <iostream>

#include <CommsWrapper.h>
#include <ABSPacket.h>
#include <CallBack.h>
#include <tools/data_structures/CAutoQueue.h>


using namespace Comnet;
using namespace Comnet::Tools::DataStructures::Interface;
using namespace Comnet::Tools::DataStructures;
using namespace std;


public ref class Testing : public ABSPacket {
public:
  Testing(int t) : cat(t), ABSPacket("Testing") { }
  Testing() : cat(0), ABSPacket("Testing") { }

  void Pack(CObjectStream^ obj) override {
      obj->input(cat);
  }

  void Unpack(CObjectStream^ obj) override {
    cat = obj->outputInt32();
  }

  ABSPacket^ Create() override {
    return gcnew Testing();
  }

  int GetCat() { return cat; }
private:
  int cat;
};

Int32 test(CHeader^ header, ABSPacket^ packet) {
  
  Testing^ testing = gcnew Testing();
  ABSPacket::GetValue<Testing^>(testing);
  cout << "This is what i got: " << testing->GetCat() << endl;
  return (Int32)CallBackCodes::CALLBACK_SUCCESS;
}

int main() {
  CQueue<Testing^>^ queueTest = gcnew CAutoQueue<Testing^>();
  CommsWrapper comm1(1);
  CommsWrapper comm2(2);


  comm1.linkCallback(gcnew Testing(), gcnew CallBack(gcnew Testing(), gcnew CommFunct(test)));

  comm1.initConnection(TransportProtocol::UDP_LINK, gcnew String("1337"), gcnew String("127.0.0.1"), 12);
  comm2.initConnection(TransportProtocol::UDP_LINK, gcnew String("1338"), gcnew String("127.0.0.1"), 0);

  comm1.addAddress(2, gcnew String("127.0.0.1"), 1338);
  comm2.addAddress(1, gcnew String("127.0.0.1"), 1337);

  comm1.run();
  comm2.run();

  while(1) {
    comm2.send(gcnew Testing(123), 1);
    Sleep(1000);    
  }
  cin.ignore();
  return 0;
}