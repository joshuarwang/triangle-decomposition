#include "decomposition-graph.h"

#include "set-util.h"

using namespace std;

decomposition_graph::decomposition_graph()
  : graph(), max_degree(0) {
}

decomposition_graph::decomposition_graph(istream& stream)
  : graph(stream), max_degree(0) {
}

void decomposition_graph::compute_data_structures() {
  compute_degrees();
  compute_triangles();
  epsilon = triangle_density() / 4;
}

bool decomposition_graph::delete_edge(int u, int v) {
  degree_to_nodes[degree(u)].erase(u);
  degree_to_nodes[degree(v)].erase(v);
  bool answer = graph::delete_edge(u, v);
  degree_to_nodes[degree(u)].insert(u);
  degree_to_nodes[degree(v)].insert(v);

  // The max degree can drop by at most one.
  if (degree_to_nodes[max_degree].empty()) --max_degree;

  // Each triangle is stored three times in indicent_triangles, once for each
  // edge. This allows us to easily remove all triangles lost by removing
  // (u, v).
  for (int w : incident_triangles[edge_key(u, v)]) {
    incident_triangles[edge_key(u, w)].erase(v);
    incident_triangles[edge_key(v, w)].erase(u);
  }
  incident_triangles.erase(edge_key(u, v));
  return answer;
}

double decomposition_graph::jaccard(int u, int v) const {
  int num_triangles = incident_triangles.count(edge_key(u, v)) ?
                        incident_triangles.at(edge_key(u, v)).size() : 0;
  return static_cast<double>(num_triangles) /
    (degree(u) + degree(v) - num_triangles);
}

bool decomposition_graph::is_empty_graph() const {
  return max_degree == 0;
}

int decomposition_graph::max_degree_vertex() const {
  return *degree_to_nodes.at(max_degree).begin();
}

void decomposition_graph::decompose(clusters* partition) {
  initial_clean();
  repeated_extraction(partition);
}

void decomposition_graph::initial_clean() {
  unordered_set<int> all_nodes;
  for (int id = 0; id < num_nodes(); ++id) {
    all_nodes.insert(get_node(id));
  }
  clean(all_nodes);
}

void decomposition_graph::repeated_extraction(clusters* partition) {
  while (!is_empty_graph()) {
    unordered_set<int> subset, dirty;
    extract(&subset, &dirty);
    partition->add_cluster(subset);
    clean(dirty);
  }
}

void decomposition_graph::clean(unordered_set<int>& dirty) {
  while (!dirty.empty()) {
    vector<pair<int, int>> edges_to_remove;
    for (int u : dirty) {
      for (int v : get_neighborhood(u)) {
        if (jaccard(u, v) < epsilon) {
          if (u < v) {
            edges_to_remove.push_back(make_pair(u, v));
          } else {
            edges_to_remove.push_back(make_pair(v, u));
          }
        }
      }
    }
    dirty.clear();
    int deleted_edges = 0;
    for (pair<int, int>& e : edges_to_remove) {
      dirty.insert(e.first);
      dirty.insert(e.second);
      if (delete_edge(e.first, e.second)) ++deleted_edges;
    }
  }
}

void decomposition_graph::extract(unordered_set<int>* subset,
                                  unordered_set<int>* dirty) {
  int v = max_degree_vertex();

  // Theta counts the number of triangles added by including key.
  unordered_map<int, int> theta;

  for (int u1 : get_neighborhood(v)) {
    for (int u2 : get_neighborhood(u1)) {
      theta[u2] = 0;
    }
  }

  for (int u1 : get_neighborhood(v)) {
    for (int u2 : set_intersect(get_neighborhood(v), get_neighborhood(u1))) {
      if (u2 <= u1) continue;
      for (int u3 : set_intersect(get_neighborhood(u1), get_neighborhood(u2))) {
        ++theta[u3];
      }
    }
  }
  ++theta[v];

  // R is the degree(v) elements in theta with the highest counts.
  unordered_set<int> R;

  // There may not even be degree(v) elements (e.g. the graph is a clique)
  // Regular Selection
  if (theta.size() <= degree(v)) {
    for (pair<const int, int>& it : theta) {
      R.insert(it.first);
    }
  } else {
    // This block of code could be modified to run in O(n) time instead of
    // O(n^2), using quickselect.
    for (int i = 0; i < degree(v); ++i) {
      pair<int, int> best = make_pair(-1, -1);
      for (const pair<const int, int>& it : theta) {
        if (it.second > best.second) best = it; 
      }
      theta.erase(best.first);
      R.insert(best.first);
    }
  }

  *subset = set_union(R, get_neighborhood(v));
  dirty->clear();
  for (int s : *subset) {
    dirty->insert(get_neighborhood(s).begin(), get_neighborhood(s).end());
  }
  // Delete all edges incident to the subset.
  for (int s : *subset) {
    while (!get_neighborhood(s).empty()) {
      delete_edge(s, *get_neighborhood(s).begin());
    }
  }
}

int decomposition_graph::num_triangles() const {
  int triple_counted_triangles = 0;
  // This triple-counts triangles since each triangle shows up once per edge.
  for (const pair<int, unordered_set<int>> it : incident_triangles) {
    triple_counted_triangles += it.second.size();
  }
  return triple_counted_triangles / 3;
}

double decomposition_graph::triangle_density() const {
  int wedges = 0;
  // We count the number of wedges by counting over the middle vertex.
  for (int i = 0; i < num_nodes(); ++i) {
    int deg = degree(get_node(i));
    wedges += deg * (deg - 1) / 2;
  }
  return static_cast<double>(3 * num_triangles()) / wedges;
}

void decomposition_graph::compute_degrees() {
  degree_to_nodes.clear();
  degree_to_nodes.resize(num_nodes());
  max_degree = 0;

  for (int id = 0; id < num_nodes(); ++id) {
    int v = get_node(id);
    degree_to_nodes[degree(v)].insert(v);
    max_degree = max(max_degree, degree(v));
  }
}

void decomposition_graph::compute_triangles() {
  incident_triangles.clear();

  for (int id = 0; id < num_nodes(); ++id) {
    int u = get_node(id);
    for (int v : get_neighborhood(u)) {
      if (v < u) continue;
      
      // The triangles incident to (u, v) can be computed via:
      // N(u) intersect N(v).
      unordered_set<int>& triangles = incident_triangles[edge_key(u, v)];
      for (int w : get_neighborhood(u)) {
        if (get_neighborhood(v).count(w)) triangles.insert(w);
      }
    }
  }
}

long long decomposition_graph::edge_key(int u, int v) const {
  if (u > v) return edge_key(v, u);
  return ((long long) get_id(u)) * num_nodes() + get_id(v);
}
