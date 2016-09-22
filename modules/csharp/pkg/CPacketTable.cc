#include <pkg/CPacketTable.h>


#define DEFAULT_ARRAY_LENGTH 1024

namespace Comnet {
namespace Pkg {


CPacketTable::CPacketTable()
: numOfPairs(0)
, tableSize(DEFAULT_ARRAY_LENGTH)
, table(gcnew cli::array<Pair^>(DEFAULT_ARRAY_LENGTH))
{
}


CPacketTable::CPacketTable(UInt32 setSize) 
: numOfPairs(0)
, tableSize(setSize)
, table(gcnew cli::array<Pair^>(setSize))
{
}


Boolean CPacketTable::Insert(ABSPacket^ key, CallBack^ callback) {
  if (!key) {
    return false;
  }

  Boolean stored = false;
  Boolean willStore = true;
  UInt32 hash = keyHash(key->GetAbstractPacket()->getId());
  Pair^ pair = gcnew Pair();
  
  pair->packet = key;
  pair->callback = callback;

  int saved = hash;
  while (table[hash] != nullptr && 
    table[hash]->packet->GetAbstractPacket()->getId() != key->GetAbstractPacket()->getId())
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


CallBack^ CPacketTable::GetCallback(UInt32 key) {
  UInt32 hash = keyHash(key);
  CallBack^ result = nullptr;

  if (table[hash]) {
    result = table[hash]->callback;
  }

  return result;
}


ABSPacket^ CPacketTable::GetPacket(UInt32 key) {
  UInt32 hash = keyHash(key);
  ABSPacket^ result = nullptr;

  UInt32 saved = hash;

  while (table[hash] && table[hash]->packet->GetAbstractPacket()->getId() != key) {
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


Boolean CPacketTable::Remove(UInt32 key) {
  return false; // for now
}


Boolean CPacketTable::Resize(UInt32 newSize) {
  return false; // for now 
}


Int32 CPacketTable::traverseIndex(Int32 i) {
  i++;
  if (i >= tableSize) {
    i = 0;
  }
  
  return i;
}


UInt32 CPacketTable::keyHash(UInt32 key) {
  return key.GetHashCode();
}
}
}