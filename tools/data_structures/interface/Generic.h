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
  /** 
      Interface Constructor.
  */
  Interface() { }
  /**
     Polymorphic Destruction.
  */
  virtual ~Interface() { }
  /**
     Grab the size of the data structure.
  */
  virtual int32_t getSize() = 0;
  /**
     get the data structure type.
  */
  virtual DataStructureType getDataStructType() = 0;
  /**
     Check if the data structure is empty.
  */
  virtual bool isEmpty() { return size == 0; }
private:
  /**
     The size of the data structure. 
  */
  int32_t size;// size of data structure
  /** 
      The type of data structure.
  */
  DataStructureType type;// enum of data structure
}

} // Package
} // DataStructure
} // Comnet

#endif // __GENERIC_H
