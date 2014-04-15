#include "set-util.h"

using namespace std;

unordered_set<int> set_intersect(const unordered_set<int>& a,
                                 const unordered_set<int>& b) {
  if (a.size() > b.size()) return set_intersect(b, a);
  unordered_set<int> result;
  for (const int i : a) {
    if (b.count(i)) result.insert(i);
  }
  return result;
}

int set_intersect_size(const std::unordered_set<int>& a,
                       const std::unordered_set<int>& b) {
  if (a.size() > b.size()) return set_intersect_size(b, a);
  int total = 0;
  for (const int i : a) total += b.count(i);
  return total;
}

unordered_set<int> set_union(const unordered_set<int>& a,
                             const unordered_set<int>& b) {
  unordered_set<int> result = a;
  result.insert(b.begin(), b.end());
  return result;
}
