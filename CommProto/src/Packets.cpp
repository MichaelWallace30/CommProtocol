#include <CommProto/Packets.h>

namespace Comnet {

/**
   When defining the constructor, be sure to call the Base class
   Constructor, the Class name SHOULD be the parameter, as a c-style string.
*/
Ping::Ping(uint32_t num) 
: num(num)
, AbstractPacket("Ping") {
 
}

Ping::Ping()
: num(10)
, AbstractPacket("Ping")
{
}


void Ping::pack(ObjectStream& obj) { 
  obj << num;
}


void Ping::unpack(ObjectStream& obj) { 
  obj >> num;
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
