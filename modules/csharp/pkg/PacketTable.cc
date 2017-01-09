#include <pkg/PacketTable.h>


#define DEFAULT_ARRAY_LENGTH 1024

namespace Comnet {
namespace Pkg {


PacketTable::PacketTable()
: numOfPairs(0)
, tableSize(DEFAULT_ARRAY_LENGTH)
, table(gcnew cli::array<Pair^>(DEFAULT_ARRAY_LENGTH))
{
}


PacketTable::PacketTable(UInt32 setSize) 
: numOfPairs(0)
, tableSize(setSize)
, table(gcnew cli::array<Pair^>(setSize))
{
}


Boolean PacketTable::Insert(ABSPacket^ key, CallBack^ callback) {
  if (!key) {
    return false;
  }

  Boolean stored = false;
  Boolean willStore = true;
  UInt32 hash = keyHash(key->GetAbstractPacket()->GetId());
  Pair^ pair = gcnew Pair();
  
  pair->packet = key;
  pair->callback = callback;

  int saved = hash;
  while (table[hash] != nullptr && 
    table[hash]->packet->GetAbstractPacket()->GetId() != key->GetAbstractPacket()->GetId())
  {
    hash = traverseIndex(hash);
    
    if (hash == saved) {
      willStore = false;
      break;
    }
  }

  if (!table[hash]) {
    table[hash] = pair;
    stored = true;
    numOfPairs++;
  } else if (willStore) {
    table[hash]->callback = callback;
    stored = true;
  }

  return stored;
}


CallBack^ PacketTable::GetCallback(UInt32 key) {
  UInt32 hash = keyHash(key);
  CallBack^ result = nullptr;

  if (table[hash]) {
    result = table[hash]->callback;
  }

  return result;
}


ABSPacket^ PacketTable::GetPacket(UInt32 key) {
  UInt32 hash = keyHash(key);
  ABSPacket^ result = nullptr;

  UInt32 saved = hash;

  while (table[hash] && table[hash]->packet->GetAbstractPacket()->GetId() != key) {
    hash = traverseIndex(hash);

    if (hash == saved) {
      break;
    }
  }

  if (table[hash]) {
    result = table[hash]->packet;
  }

  return result;
}


Boolean PacketTable::Remove(UInt32 key) {
  return false; // for now
}


Boolean PacketTable::Resize(UInt32 newSize) {
  return false; // for now 
}


UInt32 PacketTable::traverseIndex(UInt32 i) {
  i++;
  if (i >= tableSize) {
    i = 0;
  }
  
  return i;
}


UInt32 PacketTable::keyHash(UInt32 key) {
  return key.GetHashCode() % tableSize;
}
}
}