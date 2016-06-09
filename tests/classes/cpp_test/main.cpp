#include <CommProto/tools/allocator/CommPointer.h>

using namespace Comnet::Tools::Allocator;

int main(int c, char** args) {
  int cao = 5;
  CommPointer<int> ptr(&cao);
  CommPointer<int> ptr2(ptr);
  return 0;
}
