#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using std::sort;
using std::vector;
using std::upper_bound;
using std::lower_bound;
using std::distance;

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  //write your code here
  sort( starts.begin(), starts.end() );
  sort( ends.begin(), ends.end() );
  for( int i = 0; i < points.size(); ++i ){
      auto it_bound_start = upper_bound( starts.begin(), starts.end(), points[i] );
      auto it_bound_ends = lower_bound( ends.begin(), ends.end(), points[i] );
      cnt[i] = distance(starts.begin(), it_bound_start) - distance( ends.begin(), it_bound_ends);
  }
  return cnt;
}

vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < starts.size(); j++) {
      cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
    }
  }
  return cnt;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<int> starts(n), ends(n);
  for (size_t i = 0; i < starts.size(); i++) {
    std::cin >> starts[i] >> ends[i];
  }
  vector<int> points(m);
  for (size_t i = 0; i < points.size(); i++) {
    std::cin >> points[i];
  }
  //use fast_count_segments
  // vector<int> cnt = naive_count_segments(starts, ends, points);
  vector<int> cnt = fast_count_segments(starts, ends, points);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
