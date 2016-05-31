#include <stdio.h>
#include <CommProto/comms.h>
#include <architecture/os/os_threads.h>

int main(int c, char** args) {
  printf("Hello World!");
  Comms newComms = Comms();

  newComms.run();
  Sleep(1000);
  newComms.stop();
  Sleep(5000);

  return 0;
}
