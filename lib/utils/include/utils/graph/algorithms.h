#ifndef _FLEXFLOW_UTILS_GRAPH_ALGORITHMS_H
#define _FLEXFLOW_UTILS_GRAPH_ALGORITHMS_H

#include "node.h"
#include "multidigraph.h"
#include "digraph.h"
#include "undirected.h"
#include <vector>
#include <unordered_map>
#include "utils/dot_file.h"
#include "utils/containers.h"

namespace FlexFlow {
namespace utils {

std::vector<Node> add_nodes(IGraph &, int);
std::unordered_set<Node> get_nodes(IGraphView const &);

std::unordered_set<Node> query_nodes(IGraphView const &, std::unordered_set<Node> const &);

void remove_node(IMultiDiGraph &, Node const &);
void remove_node(IDiGraph &, Node const &);
void remove_node(IUndirectedGraph &, Node const &);

void remove_node_if_unused(IMultiDiGraph &, Node const &);
void remove_node_if_unused(IDiGraph &, Node const &);
void remove_node_if_unused(IUndirectedGraph &, Node const &);

void contract_node(IMultiDiGraph &, Node const &);
void contract_node(IDiGraph &, Node const &);
void contract_node(IUndirectedGraph &, Node const &);

std::size_t num_nodes(IGraphView const &);
bool empty(IGraphView const &);

void add_edges(IMultiDiGraph &, std::vector<MultiDiEdge> const &);
void add_edges(IDiGraph &, std::vector<DirectedEdge> const &);
void add_edges(IUndirectedGraph &, std::vector<UndirectedEdge> const &);

bool contains_edge(IMultiDiGraph const &, MultiDiEdge const &);
bool contains_edge(IDiGraph const &, DirectedEdge const &);
bool contains_edge(IUndirectedGraph const &, UndirectedEdge const &);

void remove_edges(IMultiDiGraph &, std::unordered_set<MultiDiEdge> const &);
void remove_edges(IDiGraph &, std::unordered_set<DirectedEdge> const &);
void remove_edges(IUndirectedGraph &, std::vector<UndirectedEdge> const &);

std::unordered_set<MultiDiEdge> get_edges(IMultiDiGraphView const &);
std::unordered_set<DirectedEdge> get_edges(IDiGraphView const &);
std::unordered_set<UndirectedEdge> get_edges(IUndirectedGraphView const &);

std::unordered_set<UndirectedEdge> get_node_edges(IUndirectedGraphView const &, Node const &);

std::unordered_set<MultiDiEdge> get_incoming_edges(IMultiDiGraphView const &, Node const &);
std::unordered_set<DirectedEdge> get_incoming_edges(IDiGraphView const &, Node const &);
std::unordered_set<MultiDiEdge> get_incoming_edges(IMultiDiGraphView const &, std::unordered_set<Node>);
std::unordered_set<DirectedEdge> get_incoming_edges(IDiGraphView const &, std::unordered_set<Node> const &);

std::unordered_set<MultiDiEdge> get_outgoing_edges(IMultiDiGraphView const &, Node const &);
std::unordered_set<MultiDiEdge> get_outgoing_edges(IMultiDiGraphView const &, std::unordered_set<Node> const &);

std::unordered_set<DirectedEdge> get_outgoing_edges(IDiGraphView const &, Node const &);
std::unordered_set<DirectedEdge> get_outgoing_edges(IDiGraphView const &, std::unordered_set<Node> const &);

std::unordered_set<UndirectedEdge> get_node_edges(IUndirectedGraphView const &, Node const &);
std::unordered_set<UndirectedEdge> get_node_edges(IUndirectedGraphView const &, std::unordered_set<Node> const &);

std::unordered_set<Node> get_predecessors(IMultiDiGraphView const &, Node const &);
std::unordered_set<Node> get_predecessors(IDiGraphView const &, Node const &);
std::unordered_map<Node, std::unordered_set<Node>> get_predecessors(IMultiDiGraphView const &, std::unordered_set<Node> const &);
std::unordered_map<Node, std::unordered_set<Node>> get_predecessors(IDiGraphView const &, std::unordered_set<Node> const &);

std::unordered_set<Node> get_sources(IDiGraphView const &);
std::unordered_set<Node> get_sources(IMultiDiGraphView const &);

std::unordered_set<Node> get_sinks(IDiGraphView const &);
std::unordered_set<Node> get_sinks(IMultiDiGraphView const &);

bool is_acyclic(IMultiDiGraphView const &, std::unordered_set<Node> const &);
tl::optional<bool> is_acyclic(IDiGraphView const &);
tl::optional<bool> is_acyclic(IMultiDiGraphView const &);

std::unordered_map<Node, std::unordered_set<Node>> dominators(IMultiDiGraphView const &);
std::unordered_map<Node, std::unordered_set<Node>> dominators(IDiGraphView const &);

/* std::vector<Node> boundary_dfs_ordering(IDiGraphView const &, std::unordered_set<Node> const &starting_points); */
std::vector<Node> dfs_ordering(IDiGraphView const &, std::unordered_set<Node> const &starting_points);
std::vector<Node> unchecked_dfs_ordering(IDiGraphView const &, std::unordered_set<Node> const &starting_points);
std::vector<Node> bfs_ordering(IDiGraphView const &, std::unordered_set<Node> const &starting_points);
std::vector<Node> topological_ordering(IDiGraphView const &);
std::vector<Node> topological_ordering(IMultiDiGraphView const &);
std::vector<Node> unchecked_topological_ordering(IDiGraphView const &);

template <typename Impl>
Impl subgraph(IUndirectedGraphView const &g, std::unordered_set<Node> const &nodes) {
  Impl result;
  for (Node const &n : query_nodes(g, nodes)) {
    result.add_node_unsafe(n);
  }
  for (UndirectedEdge const &e : get_node_edges(g, nodes)) {
    result.add_edge(e);
  }
  return result;
}

template <typename Impl>
Impl subgraph(IDiGraphView const &g, std::unordered_set<Node> const &nodes) {
  Impl result;
  for (Node const &n : query_nodes(g, nodes)) {
    result.add_node_unsafe(n);
  }
  
  std::unordered_set<DirectedEdge> subgraph_edges = set_union(
    get_incoming_edges(g, nodes), get_outgoing_edges(g, nodes)
  );

  for (DirectedEdge const &e : subgraph_edges) {
    result.add_edge(e);
  }

  return result;
}

template <typename Impl>
Impl subgraph(IMultiDiGraphView const &g, std::unordered_set<Node> const &nodes) {
  Impl result;
  for (Node const &n : query_nodes(g, nodes)) {
    result.add_node_unsafe(n);
  }

  std::unordered_set<MultiDiEdge> subgraph_edges = set_union(
    get_incoming_edges(g, nodes), get_outgoing_edges(g, nodes)
  );

  for (MultiDiEdge const &e : subgraph_edges) {
    result.add_edge(e);
  }

  return result;
}

template <typename Impl>
Impl join(IMultiDiGraphView const &lhs, IMultiDiGraphView const &rhs) {
  for (Node const &g : get_nodes(});
}

template <typename Impl>
Impl join(IDiGraphView const &g, IDiGraphView const &g) {

}

template <typename Impl>
Impl join(IUndirectedGraphView const &g, IUndirectedGraphView const &g) {

}

void export_as_dot(DotFile<Node> &, 
                   IDiGraphView const &, 
                   std::function<RecordFormatter(Node const &)> const &, 
                   tl::optional<std::function<std::string(DirectedEdge const &)> const &> = tl::nullopt);

}
}

#endif 
