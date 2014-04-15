-----------
Description
-----------

This project implements the decomposition algorithm for triangle-dense graphs
proposed by Gupta, Roughgarden, and Seshadhri (see arxiv.org/abs/1309.7440) in
C++. The code was written to read in SNAP datasets (snap.stanford.edu/data), and
outputs the resulting graph partition. See below for exact input and output
formats.

-----
Files
-----

graph.*
- Handles reading in, storing graphs, and returning basic information about a
  graph.

decomposition-graph.*
- Graph subclass that stores the necessary data structures to run the
  decomposition algorithm.

clusters.*
- Handles storage of a partition and printing the resulting partition out to a
  file.

set-util.*
- Set intersection and union functions for comparing vertex neighborhoods.

decompose.cpp
- Main function

-----
Usage
-----

make
./decompose graph_file output_file

------------------
Input Graph Format
------------------

# This is a comment line.
# Note that all ids should be 32-bit integers.
from_id1 to_id1
from_id2 to_id2
...

-----------------------
Output Partition Format
-----------------------

cluster1_node1 cluster1_node2 cluster1_node3 ...
cluster2_node1 cluster2_node2 cluster2_node3 ...
...
