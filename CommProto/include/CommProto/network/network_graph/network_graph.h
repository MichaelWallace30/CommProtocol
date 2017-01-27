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

#include <CommProto/network/network_graph/network_edge.h>
#include <CommProto/network/network_graph/network_node.h>
#include <memory>
#include <vector>
#include <unordered_map>
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

  /**
    Get the overall number of nodes in this network graph.
  */
  int32_t GetSize() { return nodes.size(); }
  
  /**
    Get the list of all nodes in this network graph.
  */
  std::vector<std::shared_ptr<Node> >& GetNodes() { return nodes; }
  
  /**
    Inserts a new node into the Network Graph. On success, function returns the 
    address of the newly created node.
  */
  Node *InsertNode(transport_protocol_t protocol, uint32_t cluster_id);

  /**
    Removes the specified node.
  */
  int32_t RemoveNode(uint32_t cluster_id);
  int32_t RemoveNode(Node *node);

  /**
    Gets the node with the specified cluster_id. Returns nullptr if no node was
    found.
  */
  Node *Search(uint32_t cluster_id);
  
  /**
    Checks if this network graph is empty.
  */
  bool IsEmpty() { return nodes.empty(); }

private:
  /**
    {cluster_id, Node_addr} key.
  */
  std::unordered_map<uint32_t, Node *> node_map;

  /**
    Node pile.
   */
  ::std::vector< ::std::shared_ptr<Node> > nodes;
  
  /*
    Checks if the Network graph requires an update.
  */
  bool dirty;
};
} // network
} // comnet
#endif // __NETWORKGRAPH_H