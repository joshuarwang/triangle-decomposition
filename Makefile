CC = g++
CFLAGS = -g -O3 -std=c++11

HEADERS = clusters.h decomposition-graph.h graph.h set-util.h
SOURCES = clusters.cpp decompose.cpp decomposition-graph.cpp graph.cpp\
					set-util.cpp
OBJECTS = $(SOURCES:.cpp=.o)
GCHHDRS = $(HEADERS:.h=.h.gch)

TARGETS = decompose

all: $(TARGETS)

clusters.o: clusters.cpp
	$(CC) $(CFLAGS) -c $^

decompose: clusters.o decompose.o decomposition-graph.o graph.o set-util.o
	$(CC) $(CFLAGS) -o $@ $^

decompose.o: decompose.cpp
	$(CC) $(CFLAGS) -c $^

decomposition-graph.o: decomposition-graph.cpp
	$(CC) $(CFLAGS) -c $^

graph.o: graph.cpp
	$(CC) $(CFLAGS) -c $^

set-util.o: set-util.cpp
	$(CC) $(CFLAGS) -c $^

Makefile.dependencies:: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $(SOURCES) > Makefile.dependencies

-include Makefile.dependencies

.PHONY: clean

clean:
	@rm -f $(TARGETS) $(OBJECTS) $(GCHHDRS) core Makefile.dependencies
