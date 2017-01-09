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
#ifndef __NETWORKGRAPH_H
#define __NETWORKGRAPH_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <memory>
#include <vector>
#include <algorithm>


namespace comnet {
namespace network {


class Node;
class Edge;


// NetworkGraph.
class NetworkGraph {
public:
  NetworkGraph();
  ~NetworkGraph();


  int32_t GetSize() { return nodes.size(); }
  std::vector<std::shared_ptr<Node> >& GetNodes() { return nodes; }
  
private:
  /**
    Node pile.
   */
  ::std::vector< ::std::shared_ptr<Node> > nodes;
  
};
} // network
} // comnet
#endif // __NETWORKGRAPH_H