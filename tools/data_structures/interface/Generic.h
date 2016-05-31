#ifndef __GENERIC_H
#define __GENERIC_H

extern "C" {
#include <architecture/os/include_defines.h>
}
namespace Comnet {
namespace DataStructures {
namespace Interface {

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
  Generic default for Information on the data structure at hand.
*/
class Generic {
public:
  /** 
      Interface Constructor.
  */
  Generic() { }
  /**
     Polymorphic Destruction.
  */
  virtual ~Generic() { }
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
protected:
  /**
     The size of the data structure. 
  */
  int32_t size;// size of data structure
  /** 
      The type of data structure.
  */
  DataStructureType type;// enum of data structure
};

} // Interface
} // DataStructure
} // Comnet

#endif // __GENERIC_H
