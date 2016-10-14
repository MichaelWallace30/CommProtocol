/*
  Generic Data structure interface for CommProtocol.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __GENERIC_H
#define __GENERIC_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

namespace comnet {
namespace tools {
namespace datastructures {
namespace interface {

/**
 Handles the data structure type.
*/
_COMNET_PRIVATE_API_
enum DataStructureType {
  NONE,
  LIST,
  QUEUE,
  HEAP,
  TREE,
  MAP,
  STACK,
};
    
/**
  Generic default for Information on the data structure At hand.
*/
_COMNET_PRIVATE_API_
class COMM_EXPORT Generic {
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
  virtual int32_t GetSize() = 0;
  /**
     Get the data structure type.
  */
  virtual DataStructureType GetDataStructType() = 0;
  /**
     Check if the data structure is empty.
  */
  virtual bool IsEmpty() { return (size == 0); }
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
} // DataStructures
} // Tools
} // Comnet

#endif // __GENERIC_H
