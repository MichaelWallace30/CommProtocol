/*
  Copyright (C) 2016  Mario Garcia, Alex Craig, Michael Wallace.

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
#ifndef __COMMPROTOCOL_NETWORK_MANAGER_H
#define __COMMPROTOCOL_NETWORK_MANAGER_H


#include <CommProto/network/network_graph/network_graph.h>
#include <CommProto/network/network_graph/algorithms/algorithms.h>
#include <CommProto/tools/observer.h>


#include <CommProto/commnode.h>


namespace comnet {


/**
  Network Manager implementation. Handles management for the network.
*/
class NetworkManager {
public:
  /**
    Network manager constructor which requires the network node.
  */
  NetworkManager(CommNode *node = nullptr);

  /**
    Set the observer for this Network Manager.
  */  
  void SetObserver(CommNode *node) { observed.Observe(node); }

  /**
    Adds a node to the Network, returns node id on success.

    @return Node id on success, otherwise -1. 
  */
  uint32_t AddNode(network::NetworkData data);

  /**
    Removes a node from the network.

    @return true on success, false if no node was found with the id.
  */
  bool RemoveNode(uint32_t id);

  /**
    Creates a segment between two nodes.
    
  @return true on success, false otherwise.
  */
  bool CreateSegment(uint32_t src, uint32_t dest);

  
  /**
    Get the data of a network node. nullptr returned if no node was 
    found with the specified id.
  */
  network::NetworkData *GetNodeData(uint32_t id);

  
  /**
    Uses Dijkstra to calculate the shortest path for all nodes in the network.
  */
  void CalculateShortestDistances();


private:
  /**
    Observer handler.
  */
  tools::Observer<CommNode, int32_t, int32_t()> observed;

  /**
    the Network graph.
  */
  network::NetworkGraph graph;
};
} // comnet
#endif // __COMMPROTOCOL_NETWORK_MANAGER_H