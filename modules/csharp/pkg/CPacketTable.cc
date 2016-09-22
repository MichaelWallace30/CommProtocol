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

}


CallBack^ CPacketTable::GetCallback(UInt32 key) {
}


ABSPacket^ CPacketTable::GetPacket(UInt32 key) {
}


Boolean CPacketTable::Remove(UInt32 key) {
  return false; // for now
}


Boolean CPacketTable::Resize(UInt32 newSize) {
  return false; // for now 
}


Int32 CPacketTable::traverseIndex(Int32 i) {
}


UInt32 CPacketTable::keyHash(UInt32 key) {
  return key.GetHashCode();
}
}
}