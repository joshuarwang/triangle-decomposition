#ifndef CLUSTERS_H
#define CLUSTERS_H

#include <fstream>
#include <unordered_set>
#include <vector>

class clusters {
 public:
  // Creates an empty list of clusters.
  clusters() {}
  // Reads cluster from the provided stream.
  void load(std::ifstream& input_stream);
  // Prints cluster to the provided stream.
  void save(std::ofstream& output_stream) const;
  
  // Adds the provided cluster to the list of clusters.
  void add_cluster(std::unordered_set<int>& cluster);
  // Sorts the clusters from largest to smallest, by magnitude.
  void sort_descending_magnitude();

  const std::vector<std::unordered_set<int>>& get_data() const;
  const int size() const;

 private:
  static bool size_compare(const std::unordered_set<int>& a,
                           const std::unordered_set<int>& b);

  std::vector<std::unordered_set<int>> data;
};

#endif  // CLUSTERS_H
