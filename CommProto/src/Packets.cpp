#include <CommProto/packets.h>

namespace comnet {

/**
   When defining the constructor, be sure to call the Base class
   Constructor, the Class name SHOULD be the parameter, as a c-style string.
*/
Ping::Ping(std::string num) 
: test(num)
, AbstractPacket("Ping") {
}

Ping::Ping()
: test("none")
, AbstractPacket("Ping")
{
}


void Ping::pack(ObjectStream& obj) { 
  obj << test;
}


void Ping::unpack(ObjectStream& obj) { 
  obj >> test;
}


AbstractPacket* Ping::create() {
  return new Ping(0);
}
// Same must be done with all other classes that extend AbstractPacket.
Pong::Pong(char letter)
: letter(letter)
, AbstractPacket("Pong") {

}


void Pong::pack(ObjectStream& obj) {
}


void Pong::unpack(ObjectStream& obj) {
}


AbstractPacket* Pong::create() {
  return new Pong(0);
}

} // Comnet
