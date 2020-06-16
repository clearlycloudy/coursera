#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>

using std::pair;
using std::vector;
using std::swap;

pair<int,int> partition3(vector<int> &a, int l, int r) {
  int x = a[l];
  int j = l;
  for (int i = l + 1; i <= r; i++) {
    if (a[i] <= x) {
      j++;
      swap(a[i], a[j]);
    }
  }
  swap(a[l], a[j]);
  int mid_start;
  for(mid_start = j - 1; mid_start >= 0; mid_start-- ){
      if( a[mid_start] != a[j] ) break;
  }
  int mid_end;
  for( mid_end = j + 1; mid_end < r; mid_end++ ){
      if( a[mid_end] != a[j] ) break;
  }
  return pair<int,int>(mid_start+1, mid_end-1);
}

void randomized_quick_sort(vector<int> &a, int l, int r) {
  if (l >= r) {
    return;
  }

  int k = l + rand() % (r - l + 1);
  swap(a[l], a[k]);
  pair<int,int> m = partition3(a, l, r);

  randomized_quick_sort(a, l, m.first - 1);
  randomized_quick_sort(a, m.second + 1, r);
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  randomized_quick_sort(a, 0, a.size() - 1);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cout << a[i] << ' ';
  }
}
