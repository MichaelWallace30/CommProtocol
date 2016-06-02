/*
  Tree Interface for CommProtocol. 
  
  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __INTERFACE_TREE_H
#define __INTERFACE_TREE_H

#include <tools/data_structures/interface/Generic.h>


namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

/**
   TreeType holds the value of what ever the generic tree may be.
 */
_COMNET_PRIVATE_API_
enum TreeType {
  BINARY_TREE,
  HEAP_TREE,
  AVL_TREE,
  RED_BLACK_TREE,
};

/**
   TreeInterface that must be inherited by all tree data structures.
*/
_COMNET_PRIVATE_API_
template<typename _Ty>
class Tree : public Generic {
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef const _Ty const_type;
public:
  Tree() {
    type = TREE;
  }

  virtual ~Tree() { }

  

protected:
  TreeType treeType;
};
} // Interface namespace 
} // DataStructures namespace
} // Tools namespace 
} // Comnet namespace
#endif // __INTERFACE_TREE_H
