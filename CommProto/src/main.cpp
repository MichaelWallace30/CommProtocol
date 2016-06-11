#include <stdio.h>
#include <CommProto/comms.h>
#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/architecture/os/include_defines.h>


int main(int c, char** args) {
  printf("Hello World!");
  Comms newComms1 = Comms(1);
  Comms newComms2 = Comms(2);
  newComms1.initConnection(SERIAL_LINK, "COM2", "", 9600);
  newComms2.initConnection(SERIAL_LINK, "COM3", "", 9600);
  

  newComms1.addAddress(2);
  newComms2.addAddress(1);


	uint8_t buf[1024] = { 1 };
	newComms1.send(2, 1, buf, 10);	    

	newComms2.run();
	newComms1.run();

	  uint8_t sourceID;
	  uint16_t messageID;
	  uint32_t messageLength;
	  uint8_t buffer[10];
	  while (true)
	  {


	  }
	return 0;
}
