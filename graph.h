#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <map>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Class to represent an unweighted, undirected graph.
class graph {
 public:
  graph() {}
  explicit graph(std::istream& stream);
  virtual ~graph() {}

  void get_nodes(std::vector<int>* nodes) const;
  void get_edges(std::vector<std::pair<int, int>>* edges) const;
  void add_edge(int u, int v);
  virtual bool delete_edge(int u, int v);

  const std::unordered_set<int>& get_neighborhood(int v) const;
  int degree(int v) const;

  int num_nodes() const;
  int num_edges() const;

  double edge_density() const;

 private:
  friend class decomposition_graph;

  int get_id(int v);
  int get_id(int v) const;
  int get_node(int i) const;

  std::unordered_map<int, int> node_to_id;
  std::unordered_map<int, int> id_to_node;
  // The ith vector entry stores the neighborhood of vertex i.
  // The ith vector entry stores the neighborhood of the vertex with id i.
  std::vector<std::unordered_set<int>> neighborhood;
};

#endif  // GRAPH_H
