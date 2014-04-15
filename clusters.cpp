#include "clusters.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>

#include "set-util.h"

using namespace std;

void clusters::load(ifstream& input_stream) {
  while (true) {
    string line; getline(input_stream, line);
    if (input_stream.fail() || input_stream.eof()) break;
    if (line.empty()) continue;
    istringstream line_stream(line);

    data.push_back(unordered_set<int>());
    unordered_set<int>& curr = data.back();
    while (true) {
      int node;
      line_stream >> node;
      if (line_stream.fail() || line_stream.eof()) break;
      curr.insert(node);
    }
  }
}

void clusters::save(ofstream& output_stream) const {
  for (const unordered_set<int>& clusters : data) {
    for (const int node : clusters) {
      output_stream << node << " ";
    }
    output_stream << endl;
  }
}

void clusters::add_cluster(unordered_set<int>& cluster) {
  data.push_back(cluster);
}

void clusters::sort_descending_magnitude() {
  sort(data.begin(), data.end(), size_compare);
}

const vector<unordered_set<int>>& clusters::get_data() const {
  return data;
}

const int clusters::size() const {
  return data.size();
}

bool clusters::size_compare(const unordered_set<int>& a,
                            const unordered_set<int>& b) {
  return a.size() > b.size();
}
