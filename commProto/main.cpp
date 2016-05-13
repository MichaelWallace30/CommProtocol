#include <stdio.h>
#include "comms.h"

int main(int c, char** args) {
  printf("Hello World!");

  Comms newComms = Comms();

  newComms.run();
  Sleep(1000);
  newComms.stop();
  Sleep(1000);

  return 0;
}
