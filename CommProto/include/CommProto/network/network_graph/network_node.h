/*
  Network node.

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
#ifndef __NETWORKNODE_H
#define __NETWORKNODE_H


#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/network/commslink.h>
#include <CommProto/network/network_graph/network_edge.h>
#include <memory>


namespace comnet {
namespace network {

class NetworkGraph;
typedef uint32_t cluster_t;


/**
  Checks the status of the network for a given node.
*/
enum NetworkStatus {
  NETWORK_UNKNOWN,
  NETWORK_OFFLINE,
  NETWORK_ONLINE,
  NETWORK_DOWN,
  NETWORK_BLOCKED
};


/**
  String value of the network name.
*/
struct NetworkName {
  uint32_t size;
  char name[32];
};

/**
  Network data.
*/
struct NetworkData {
  uint32_t              latency;
  cluster_t             cluster_id;
  transport_protocol_t  protocol;
  NetworkStatus         status;
  NetworkName           name;
};


// Network node definitions.
class Node {
public:
  Node();

  ~Node();

  bool AddEdge(Node *dest);
  bool RemoveEdge(Node *dest);
  
  Edge* GetEdge(Node *dest);
  
  uint32_t GetLatency() const { return m_data.latency; }
  transport_protocol_t GetProtocol() const { return m_data.protocol; }
  NetworkStatus GetNetworkStatus() const { return m_data.status; }
  uint32_t GetClusterId() const { return m_data.cluster_id; } 
  NetworkName GetNetworkName() const { return m_data.name; }

  bool SwapData(NetworkData &data) { 
    std::swap(data, m_data); 
  }

  int32_t GetId() const { return id; }

  void SetLatency(int32_t c) { m_data.latency = c; }
  void SetNetworkStatus(NetworkStatus status) { m_data.status = status; }
  void SetNetworkProtocol(transport_protocol_t proto) { m_data.protocol = proto; }
  void SetClusterId(uint32_t id) { m_data.cluster_id = id; }
  void SetNetworkName(const char *name);

  std::vector<std::shared_ptr<Edge>> &GetOutgoing() { return outgoing.GetEdges(); }
  std::vector<std::shared_ptr<Edge>> &GetIncoming() { return incoming.GetEdges(); } 
 
private:
  EdgeContainer outgoing;
  EdgeContainer incoming;
  // Network data.
  NetworkData m_data;
  /**
    Identification of the node.
   */
  int32_t id;

  // friendly neighborhood NetworkGraph...
  friend class NetworkGraph;
};
} // network
} // comnet 
#endif // __NETWORKNODE_H