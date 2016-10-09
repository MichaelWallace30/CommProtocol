#include <iostream>

#include <Comms.h>
#include <ABSPacket.h>
#include <CallBack.h>
#include <tools/data_structures/AutoQueue.h>


using namespace Comnet;
using namespace Comnet::Tools::DataStructures::Interface;
using namespace Comnet::Tools::DataStructures;
using namespace std;


public ref class Testing : public ABSPacket {
public:
  Testing(int t) : cat(t), ABSPacket("Testing") { }
  Testing() : cat(0), ABSPacket("Testing") { }

  void Pack(Comnet::Serialization::ObjectStream^ obj) override {
      obj->Input(cat);
  }

  void Unpack(Comnet::Serialization::ObjectStream^ obj) override {
    cat = obj->OutputInt32();
  }

  ABSPacket^ Create() override {
    return gcnew Testing();
  }

  int GetCat() { return cat; }
private:
  int cat;
};

Int32 test(Comnet::Header^ header, ABSPacket^ packet) {
  Testing^ testing = ABSPacket::GetValue<Testing^>(packet);
  cout << "This is what i got: " << testing->GetCat() << endl;
  return (Int32)CallBackCodes::CALLBACK_SUCCESS;
}

int main() {
  Comnet::Tools::DataStructures::Interface::Queue<Testing^>^ queueTest = 
            gcnew Comnet::Tools::DataStructures::AutoQueue<Testing^>();
  queueTest->EnQueue(gcnew Testing(11));
  queueTest->EnQueue(gcnew Testing(12333));
  Testing^ ss = queueTest->DeQueue();
  Console::WriteLine(gcnew String("This is cat: " + ss->GetCat()));
  Comms comm1(1);


  comm1.LinkCallback(gcnew Testing(), gcnew Comnet::CallBack(gcnew CommFunct(test)));

  comm1.InitConnection(TransportProtocol::UDP_LINK, gcnew String("1337"), gcnew String("127.0.0.1"), 12);

  comm1.AddAddress(2, gcnew String("127.0.0.1"), 1338);

  comm1.Run();

  while(1) {
    comm1.Send(gcnew Testing(123), 1);
    Sleep(1000);    
  }
  cin.ignore();
  return 0;
}