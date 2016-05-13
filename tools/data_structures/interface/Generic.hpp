#ifndef __GENERIC_H
#define __GENERIC_H


#include <architecture/os/include_defines.h>

namespace Comnet {
namespace DataStructure {
namespace Package {

/**
 Handles the data structure type.
*/
enum DataStructureType {
  NONE,
  LIST,
  QUEUE,
  HEAP,
  TREE,
};
    
/**
  Interface default for Information on the data structure at hand.
*/
class Interface {
public:
  Interface() { }
  virtual ~Interface() { }
  
  virtual int32_t getSize() = 0;
  virtual DataStructureType getDataStructType() = 0;
  virtual bool isEmpty() { return size == 0; }
private:
  int32_t size;// size of data structure
  DataStructureType type;// enum of data structure
}

} // Package
} // DataStructure
} // Comnet

#endif // __GENERIC_H
