/*
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
#ifndef __NETWORKEDGE_H
#define __NETWORKEDGE_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>

#include <memory>
#include <algorithm>
#include <vector>

namespace comnet {
namespace network {

class Node;


// Working on edges and nodes.
class Edge {
  COMM_DISALLOW_COPYING(Edge);

public:
  Edge(Node* src, Node* dest, int32_t dist) 
  : src(src)
  , dest(dest)
  , dist(dist) { }

  Edge()
  : src(NULL)
  , dest(NULL)
  , dist(-1) { }
  
  ~Edge() { }

  Node* GetSrc() { return src.get(); }
  Node* GetDest() { return dest.get(); }
  int32_t GetDist() { return dist; }

  void ReLinkSrc(Node* new_src) { src.reset(new_src); }
  void ReLinkDest(Node* new_dest) { dest.reset(new_dest); } 
  void SetDist(int32_t new_dist) { dist = new_dist; }
private:
  /**
    Source node.
   */  
  ::std::shared_ptr<Node> src;
  /**
    Destination node.
   */
  ::std::shared_ptr<Node> dest;
  /**
    Distance weight between the destination and source nodes.
  */  
  int32_t dist;
};


// Contains edges.
class EdgeContainer {
public:
  EdgeContainer() { } 
  ~EdgeContainer() { }

  bool AddEdge(Node* src, Node* dest);
  bool RemoveEdge(Node* src, Node* dest);
  
  Edge* GetEdge(Node* src, Node* dest);
  bool ReattachEdge(Node* src, Node* dest, Node* new_dest);

  std::vector< ::std::shared_ptr<Edge>> &GetEdges() { return edges; } 
  
private:
  // List of shared pointers. 
  ::std::vector< ::std::shared_ptr<Edge> > edges;
};
} // network
} // comnet
#endif // __NETWORKEDGE_H