#include <fstream>

#include "clusters.h"
#include "decomposition-graph.h"

using namespace std;

// Handles parsing arguments from the command line. Returns true on success.
// graph_file and output_file should be non-NULL.
bool get_arguments(int argc, char *argv[], string* graph_file,
                   string* output_file) {
  if (argc != 3) {
    printf("Usage: %s graph_file output_file\n", argv[0]);
    return false;
  }
  *graph_file = argv[1];
  *output_file = argv[2];
  return true;
}

int main(int argc, char *argv[]) {
  string graph_file;
  string output_file;
  if (!get_arguments(argc, argv, &graph_file, &output_file)) return -1;

  ifstream graph_stream(graph_file);
  decomposition_graph g(graph_stream);
  g.compute_data_structures();

  clusters partition;
  g.decompose(&partition);
  ofstream output_stream(output_file);
  partition.save(output_stream);

  return 0;
}
