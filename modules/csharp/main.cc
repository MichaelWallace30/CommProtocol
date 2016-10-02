#include <iostream>

#include <CComms.h>
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
      obj->Input(cat);
  }

  void Unpack(CObjectStream^ obj) override {
    cat = obj->OutputInt32();
  }

  ABSPacket^ Create() override {
    return gcnew Testing();
  }

  int GetCat() { return cat; }
private:
  int cat;
};

Int32 test(CHeader^ header, ABSPacket^ packet) {
  
  Testing^ testing = ABSPacket::GetValue<Testing^>(packet);
  cout << "This is what i got: " << testing->GetCat() << endl;
  return (Int32)CallBackCodes::CALLBACK_SUCCESS;
}

int main() {
  CQueue<Testing^>^ queueTest = gcnew CAutoQueue<Testing^>();
  queueTest->EnQueue(gcnew Testing(11));
  queueTest->EnQueue(gcnew Testing(12333));
  Testing^ ss = queueTest->DeQueue();
  Console::WriteLine(gcnew String("This is cat: " + ss->GetCat()));
  CComms comm1(1);
  CComms comm2(2);


  comm1.LinkCallback(gcnew Testing(), gcnew CallBack(gcnew Testing(), gcnew CommFunct(test)));

  comm1.InitConnection(TransportProtocol::UDP_LINK, gcnew String("1337"), gcnew String("127.0.0.1"), 12);
  comm2.InitConnection(TransportProtocol::UDP_LINK, gcnew String("1338"), gcnew String("127.0.0.1"), 0);

  comm1.AddAddress(2, gcnew String("127.0.0.1"), 1338);
  comm2.AddAddress(1, gcnew String("127.0.0.1"), 1337);

  comm1.Run();
  comm2.Run();

  while(1) {
    comm2.Send(gcnew Testing(123), 1);
    Sleep(1000);    
  }
  cin.ignore();
  return 0;
}