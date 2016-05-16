/* NOTE: This is only a rough implementation. Not the real Thing!! */
#ifndef __ABSTRACT_PACKET_H
#define __ABSTRACT_PACKET_H

#include <CommProto/CommsPacket.h>

/** Use this AbstractPacket when it comes to packing, sending , and receiving */
class AbstractPacket {
protected:
  PacketType packType;
  
public:
  AbstractPacket() { }
  virtual ~AbstractPacket() { }
};

// Inherit AbstractPacket for EVERY specified packet class that user programs. */
class CommsPacket : public AbstractPacket {
private:
  int id;
  bool isCool;
public:
  CommsPacket() : id(0), isCool(false) { packType = COMMS_PACKET; }
  CommsPacket(int id, bool cool) : id(id), isCool(cool) { packType = COMMS_PACKET; }

  int performCalculations(int a);
  int getId() { return id; }
};

// Test Ping Packet
class PingPacket : public AbstractPacket { 
private:
  char kitten;
public:
  PingPacket() : kitten('k') { packType = PING_PACKET; }
  PingPacket(char kitten) : kitten(kitten) { packType = PING_PACKET; }
  char getKitten() { return kitten; }
};

// Test Pong Packet
class PongPacket : public AbstractPacket {
private:
  char kitten;
public:
  PongPacket() : kitten('r') { packType = PONG_PACKET; }
  
  char getKitten() { return kitten; }

};
#endif // __ABSTRACT_PACKET_H
