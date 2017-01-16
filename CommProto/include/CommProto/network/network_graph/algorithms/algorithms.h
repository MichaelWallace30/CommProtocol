/**
  
*/
#ifndef __COMMPROTOCOL_ALGORITHMS_H
#define __COMMPROTOCOL_ALGORITHMS_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/tools/standard_comparator.h>
#include <CommProto/network/network_graph/network_graph.h>
#include <CommProto/network/network_graph/network_edge.h>
#include <CommProto/network/network_graph/network_node.h>
#include <functional>
#include <queue>
#include <set>

namespace comnet {
namespace network {
namespace algorithms {


/**
  BFS implementation to search for a cluster within the network graph.
*/
template<typename ClusterCompare,
         typename Callback = std::function<void()> >
void BFS(NetworkGraph *graph, Node *root, CommsLink &target, Callback handler) {
  ClusterCompare compare;
  std::vector<Node*> visited;
  std::queue<Node*> queue;
  queue.push(root);
  while (!queue.empty()) {
    Node *node = queue.front();
    queue.pop();
    if (std::find(visited.begin(), visited.end(), node) == visited.end()) {
      std::vector<Edge *> edges = node->GetOutgoing();
      if (compare(node->GetCluster(), target)) {
        handler(target);
        break;
      }
      for (int i = 0; i < edges.size(); ++i) {
        Node *dst = edges.at(i)->GetDest();
        queue.push(dst);
      }
    }
  }
}


/**
  Modified Dijkstra Algorithm to calculate the shortest distances from source to
  all nodes in the graph. It is not greedy, which may be a problem as it may reach
  average case big O (|E| + |V| log |V|), where E is the number of edges and V is the numer of vertices,
  which is good case considering the use of binary heap as the queue.

  Implementation is not tested! We need to test later on... Handler is not used yet.
*/
void Dijkstra(NetworkGraph *graph, Node *source) {
  std::vector<std::shared_ptr<Node> > nodes = graph->GetNodes();
  std::set<Node *> visited;
  auto cmp = [] (Node *node1, Node *node2) { return node1->GetLatency() < node2->GetLatency(); };
  std::priority_queue<Node *, std::vector<Node *>, decltype(cmp)> unvisited(cmp);
  for (int i = 0; i < nodes.size(); ++i) {
    if (nodes.at(i).get() != source) {
      // Windows windef.h has a macro under the name 'max', workaround is to use paranthesis around
      // numeric limits.
      nodes.at(i)->SetLatency((std::numeric_limits<int32_t>::max)());
    }
    unvisited.push(nodes.at(i).get());
  }
  source->SetLatency(0);
  while (!unvisited.empty()) {
    Node *current = unvisited.top();
    unvisited.pop();
    visited.insert(current);
    std::vector<std::shared_ptr<Edge> > &edges = current->GetOutgoing();
    for (uint32_t i = 0; i < edges.size(); ++i) {
      Edge *edge = edges[i].get();
      Node *t = edge->GetDest();
      if (visited.find(t) == visited.end()) {
        if (current->GetLatency() + edge->GetDist() < t->GetLatency()) {
          t->SetLatency(current->GetLatency() + edge->GetDist());
        }
      }
    }
  }
}
} // algorithms
} // network
} // comnet
#endif // __COMMPROTOCOL_ALGORITHMS_H