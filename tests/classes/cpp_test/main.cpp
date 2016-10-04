#include <CommProto/comms.h>

int main(int c, char** args) {
  std::cout << "I am a cat" << std::endl;
  comnet::Comms comm1(1);
  std::cin.ignore();
  return 0;
}
