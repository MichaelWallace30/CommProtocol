#include <stdio.h>
#include <CommProto/comms.h>
#include <CommProto/architecture/os/os_threads.h>
#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/tools/hash/NgcpHash.h>

using namespace Comnet::Tools::Hash;

int testHash() {
  unsigned long long start = 5000000;
  unsigned long long end = 10000000;
  int counter = 0;
  for(unsigned long long i = start; i < end; ++i) {
    ngcp_hash64_t hash = ngcp_hash64(&i, 8, 0);
    for(unsigned long long j = start; j < end; ++j) {
      ngcp_hash64_t test = ngcp_hash64(&j, 8, 0);
      if(test == hash) {
        if(i != j) {
          std::cout << "COLLISION: i: " << i << " j: " << j << std::endl;
          counter++;
        }
      }
    }
  }
  printf("FINISHED\n# OF COLLISIONS: %lu\n", counter);
  printf("COLLISION CHANGE : %f%%", (counter / (end - start))); 
  return 0;
}

int testCharHash() {
  std::string word = "CommProtPackfwjiofwoifwoijfoieifwoeifoijweoijfwoijfoijewoewfwefwfwfweetwefwefwefwefewfwefww";
  std::string test = "AbstractPackeweifowefweiojfijoweiofioweioiojewfwefwefwefwetwefwfe";
  uint64_t wordHash = ngcp_hash64((void*)word.c_str(), word.size(), 0);
  uint64_t testHash = ngcp_hash64((void*)test.c_str(), test.size(), 0);
  std::cout << wordHash << std::endl;
  std::cout << testHash << std::endl;

  if (wordHash == testHash) {
    std::cout << "COLLISION" << std::endl;
  }
  return 0;
} 

int main(int c, char** args) {
  printf("Hello World!\n");
  testCharHash();
/*
  Comms newComms1 = Comms(1);
  Comms newComms2 = Comms(2);
  newComms1.initConnection(UDP_LINK, "1337", "127.0.0.1");
  newComms2.initConnection(UDP_LINK, "1338", "127.0.0.1");
  

  newComms1.addAddress(2, "127.0.0.1", 1338);
  newComms2.addAddress(1, "127.0.0.1", 1337);


	uint8_t buf[1024] = { 1 };
	newComms1.send(2, 1, buf, 10);	    

	newComms2.run();
	newComms1.run();
*/

  Comms newComms1 = Comms(1);
  Comms newComms2 = Comms(2);
  newComms1.initConnection(SERIAL_LINK, "/dev/ttyS0", "", 115200);
  newComms2.initConnection(SERIAL_LINK, "/dev/ttyS1", "", 115200);


  newComms1.addAddress(2);
  newComms2.addAddress(1);
  std::cin.ignore();

  uint8_t buf[1024] = {1};
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
