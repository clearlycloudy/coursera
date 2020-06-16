#include <iostream>
#include <vector>

using namespace std;

long long get_number_of_inversions(vector<int> &a, vector<int> &b, size_t left, size_t right) {
  long long number_of_inversions = 0;
  if (right <= left + 1) return number_of_inversions;
  size_t ave = left + (right - left) / 2;
  number_of_inversions += get_number_of_inversions(a, b, left, ave);
  number_of_inversions += get_number_of_inversions(a, b, ave, right);
  //write your code here

  //merge and count number of inversions
  int i = left;
  int j = ave;
  int k = left;
  while( (i < ave) && (j < right) ){
      if( a[j] < a[i] ){
	  b[k] = a[j];
	  number_of_inversions += (ave-i);
	  j++;
      }
      else{
	  b[k] = a[i];
	  i++;
      }
      k++;
  }
  for( int h = i; h < ave; ++h ){
      b[k] = a[h];
      k++;
  }
  for( int h = j; h < right; ++h ){
      b[k] = a[h];
      k++;
  }
  for( int h = left; h < right; ++h ){
      a[h] = b[h];
  }
  return number_of_inversions;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  vector<int> b(a.size());
  std::cout << get_number_of_inversions(a, b, 0, a.size()) << '\n';
  // for( auto & i : b ){
  //     cout << i << " ";
  // }
  // cout << endl;
}
