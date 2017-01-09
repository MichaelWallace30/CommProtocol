/*
  Runs a thread to update pingers.

  Copyright(C) 2016  Alex Craig, Michael Wallace, Mario Garcia.

  This program is free software : you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __COMM_RBTREE_H
#define __COMM_RBTREE_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <CommProto/tools/data_structures/interface/interface_tree.h>
#include <CommProto/tools/allocator/newallocator.h>
#include <CommProto/tools/standard_comparator.h>


namespace comnet {
namespace tools {
namespace datastructures {


/**
  Red-Black Tree implementation, which is a data structure for completely 
  fair balancing. Perfect for scheduling and what-not, notably because of its
  O (log n) time complexity for insertion, deletion, and look up. 
*/
template<typename Object,
         typename Allocator   = allocator::NewAllocator<Object>,
         typename Comparator  = StandardComparator<Object> >
class RBTree : public interface::Tree<Object> {
  /**
    Color scheme for the red black tree.
  */
  enum Color {
    RED,
    BLACK
  };

  /**
    Standard Red-Black node for this Red-Black tree.
  */
  struct RBNode {
    RBTree    *ref;
    RBNode    *left;
    RBNode    *right;
    RBNode    *parent;
    Color     color;
    Object    data;
  };
public:
  

private:
  /**
    Starting root of the tree.
  */
  RBNode *root;
};
} // datastructures
} // tools
} // comnet
#endif // __COMM_RBTREE_H