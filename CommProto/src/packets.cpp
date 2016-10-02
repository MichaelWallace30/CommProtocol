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


void Ping::Pack(ObjectStream& obj) { 
  obj << test;
}


void Ping::Unpack(ObjectStream& obj) { 
  obj >> test;
}


AbstractPacket* Ping::Create() {
  return new Ping();
}
// Same must be done with all other classes that extend AbstractPacket.
Pong::Pong(char letter)
: letter(letter)
, AbstractPacket("Pong") {

}


void Pong::Pack(ObjectStream& obj) {
}


void Pong::Unpack(ObjectStream& obj) {
}


AbstractPacket* Pong::Create() {
  return new Pong(0);
}

} // Comnet
