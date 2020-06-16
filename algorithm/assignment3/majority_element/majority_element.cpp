#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

int get_majority_element(vector<int> &a, int left, int right) {
  if (left == right) return -1;
  if (left + 1 == right) return a[left];
  //write your code here
  unordered_map<int,int> freq;
  for( int i = left; i < right; ++i ){
      if( freq.find(a[i]) == freq.end() ){
	  freq[a[i]] = 0;
      }
      freq[a[i]]++;
  }
  int countmax = 0;
  for( auto it = freq.begin(); it != freq.end(); ++it ){
      if( it->second > countmax ){
	  countmax = it->second;
      }
  }
  if( countmax < (right-left)/2 + 1){
      return -1;
  }else{
      return 1;
  }
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
