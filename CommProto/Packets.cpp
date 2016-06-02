#include <Packets.h>

namespace Comnet {

/**
   When defining the constructor, be sure to call the Base class
   Constructor, the Class name SHOULD be the parameter, as a c-style string.
*/
Ping::Ping(uint32_t num) 
: num(num)
, AbstractPacket("Ping") {
 
}

void Ping::pack(ObjectStream& obj) { 
}
void Ping::unpack(ObjectStream& obj) { 
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

} // Comnet
