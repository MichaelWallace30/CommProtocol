#include <CommProto/comms.h>
#include <CommProto/callback.h>
#include <CommProto/architecture/os/comm_mutex.h>
#include <iostream>


class Ping : public comnet::AbstractPacket {
public:
  Ping() : comnet::AbstractPacket("Ping") { }
  

  void Pack(comnet::ObjectStream& obj) {
  }

  void Unpack(comnet::ObjectStream& obj) {
  }
  
  comnet::AbstractPacket* Create() {
    return new Ping();
  }
private:
};


int main(int c, char** args) {
  std::cout << sizeof(comnet::Header) << std::endl;
  comnet::architecture::os::CommMutex mut;
  comnet::architecture::os::CommLock commlock(mut);
  comnet::Comms comm1(1);

  
  std::cout << "Test complete!" << std::endl;
  std::cin.ignore();
  return 0;
}
