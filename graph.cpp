#include "graph.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

graph::graph(istream& stream) {
  int u, v;
  string line;
  while (true) {
    getline(stream, line);
    if (stream.eof() || stream.fail()) break;
    // Skip comment lines and empty lines.
    if (line.empty() || line[0] == '#') continue;
    stringstream ss(line);
    ss >> u >> v;
    if (ss.fail()) break;
    add_edge(u, v);
  }
}

void graph::get_nodes(vector<int>* nodes) const {
  nodes->clear();
  for (int id = 0; id < neighborhood.size(); ++id) {
    nodes->push_back(get_node(id));
  }
}

void graph::get_edges(vector<pair<int, int>>* edges) const {
  edges->clear();
  for (int id = 0; id < neighborhood.size(); ++id) {
    int u = get_node(id);
    for (const int v : neighborhood[id]) {
      if (u < v) edges->push_back(make_pair(u, v));
    }
  }
}

void graph::add_edge(int u, int v) {
  if (u == v) return;
  int uid = get_id(u);
  int vid = get_id(v);
  if (max(uid, vid) >= neighborhood.size())
    neighborhood.resize(max(uid+1, vid+1));
  neighborhood[uid].insert(v);
  neighborhood[vid].insert(u);
}

bool graph::delete_edge(int u, int v) {
  int uid = get_id(u);
  int vid = get_id(v);
  bool answer = neighborhood[uid].count(v);
  neighborhood[uid].erase(v);
  neighborhood[vid].erase(u);
  return answer;
}

const unordered_set<int>& graph::get_neighborhood(int v) const {
  return neighborhood[get_id(v)];
}

int graph::degree(int v) const {
  return neighborhood[get_id(v)].size();
}

int graph::num_nodes() const {
  return neighborhood.size();
}

int graph::num_edges() const {
  int double_counted_edges = 0;
  // This double-counts edges since each edge shows up once per endpoint.
  for (const unordered_set<int>& n : neighborhood) {
    double_counted_edges += n.size();
  }
  return double_counted_edges / 2;
}

double graph::edge_density() const {
  return num_edges() / (0.5*num_nodes()*(num_nodes()-1));
}

int graph::get_id(int v) {
  if (!node_to_id.count(v)) {
    node_to_id.insert(make_pair(v, node_to_id.size()));
    id_to_node.insert(make_pair(id_to_node.size(), v));
  }
  return node_to_id[v];
}

int graph::get_id(int v) const {
  return node_to_id.count(v) ? node_to_id.at(v) : -1;
}

int graph::get_node(int i) const {
  if (!id_to_node.count(i)) return -1;
  return id_to_node.at(i);
}
