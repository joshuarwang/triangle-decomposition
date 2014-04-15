#ifndef SET_UTIL_H
#define SET_UTIL_H

#include <unordered_set>

// Returns A intersect B.
std::unordered_set<int> set_intersect(const std::unordered_set<int>& a,
                                      const std::unordered_set<int>& b);

// Returns the size of A intersect B.
int set_intersect_size(const std::unordered_set<int>& a,
                       const std::unordered_set<int>& b);

// Returns A union B.
std::unordered_set<int> set_union(const std::unordered_set<int>& a,
                                  const std::unordered_set<int>& b);

#endif  // SET_UTIL_H
