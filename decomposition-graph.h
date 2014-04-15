// Graph data structure that stores information about triangle counts needed to
// run the decomposition algorithm.
//
// Sample Usage:
//
// ifstream graph_stream(graph_file);
// decomposition_graph g(graph_stream);
// g.compute_data_structures();
//
// clusters partition;
// g.decompose(&partition);

#ifndef DECOMPOSITION_GRAPH_H
#define DECOMPOSITION_GRAPH_H

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "clusters.h"
#include "graph.h"

class decomposition_graph : public graph {
 public:
  decomposition_graph();
  decomposition_graph(std::istream& stream);
  virtual ~decomposition_graph() {}

  // Recomputes the data structures kept by the graph.
  // Should be called after adding edges.
  void compute_data_structures();

  virtual bool delete_edge(int i, int j);

  double jaccard(int u, int v) const;
  bool is_empty_graph() const;
  int max_degree_vertex() const;

  // Decomposition does an initial clean and then repeated extraction of
  // clusters. Both phases are provided in case the caller wishes to analyze the
  // graph between the two phases.
  void decompose(clusters* partition);
  // Removes all edges whose jaccard similarity is below epsilon.
  void initial_clean();
  // Repeatedly extracts and then cleans until no edges remain. Stores the
  // found clusters in partition, which should be non-NULL.
  void repeated_extraction(clusters* partition);
  void clean(std::unordered_set<int>& dirty);
  void extract(std::unordered_set<int>* subset, std::unordered_set<int>* dirty);

  int num_triangles() const;
  double triangle_density() const;

 private:
  void compute_degrees();
  void compute_triangles();

  long long edge_key(int u, int v) const;

  // The ith vector entry stores indicies of all nodes with degree i.
  std::vector<std::unordered_set<int>> degree_to_nodes;
  // The largest degree among all nodes of the graph.
  int max_degree;
  // The set of all triangles incident to edge (u, v).
  // The key for edge (u, v), is given by edge_key() above.
  std::unordered_map<long long, std::unordered_set<int>> incident_triangles;
  // A decomposition parameter, to be set to the initial triangle density/4.
  double epsilon;
};

#endif  // DECOMPOSITION_GRAPH_H
