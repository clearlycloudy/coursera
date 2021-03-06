#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using std::deque;
using std::map;
using std::unordered_map;
using std::unordered_set;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.

vector<int> stable_sort_char( int total_order, vector<int> in ){
    vector<int> order(in.size()); //ascending order of elements in the input array
    
    vector<int> num_count(total_order,0);

    for( int i = 0; i < in.size(); ++i ){
	num_count[in[i]]++;
    }

    for( int i = 1; i < num_count.size(); ++i ){
	num_count[i] += num_count[i-1];
    }

    for( int i = in.size() - 1; i >= 0; --i ){
	num_count[ in[i] ]--;
        order[ num_count[ in[i] ] ] = i;
    }

    return order;
}



vector<int> sort_doubled( int n, int l, vector<int> order, vector<int> classes, int num_classes ){
    assert( n == order.size() );
    vector<int> count(n,0);
    vector<int> update_order(n);

    for( int i = 0; i < n; ++i ){
	count[classes[i]] = count[classes[i]] + 1;
    }
    for( int i = 1; i < n; ++i ){
	count[i] += count[i-1];
    }
    for( int i = n-1; i >= 0; --i ){
	int start = ( order[i] + n - l ) % n; //cyclic shift l mod n
	assert( order[i] + n - l  >= 0 );
	assert( start >= 0 );
	int cl = classes[start];
	count[cl] = count[cl] - 1;
	update_order[count[cl]] = start;
    }
    return update_order;
}
std::pair<vector<int>,int> update_classes( vector<int> order, vector<int> classes, int l, int num_classes ){
    int n = order.size();
    vector<int> classes_update(n);
    int count_misses = 0;
    classes_update[order[0]] = 0;
    for( int i = 1; i < n; ++i ){
	int cur = order[i];
	int prev = order[i-1];
	int mid = (cur + l) % n;
	int mid_prev = (prev + l) % n;
	if( (classes[cur] != classes[prev]) ||
	    (classes[mid] != classes[mid_prev]) ){
	    classes_update[cur] = classes_update[prev] + 1;
	    count_misses++;
	} else {
	    classes_update[cur] = classes_update[prev];
	}
    }

    int max_num = 0;
    for( int i = 0; i < classes_update.size(); ++i ){
	if( max_num < classes_update[i] ){
	    max_num = classes_update[i];
	}
    }
    // std::cout << "max_num: " << max_num << ", count_misses: " << count_misses << ", num_classes: " << num_classes << std::endl;
    // assert( max_num + 1 == count_misses + num_classes );
    num_classes = max_num + 1;
    
    return { classes_update, num_classes };
}

std::pair<vector<int>,int> compute_classes( string s, vector<int> order ){
    vector<int> classes(s.size());
    int num_classes = 0;
    classes[order[0]] = 0;
    for( int i = 1; i < s.size(); ++i){
	if( s[order[i]] != s[order[i-1]] ){
	    // cout << "char not equal: " << s[order[i]] << ", " << s[order[i-1]] << endl;
	    classes[order[i]] = classes[order[i-1]] + 1;
	} else {
	    classes[order[i]] = classes[order[i-1]];
	    // cout << "char equal: " << s[order[i]] << endl;
	}
	if( num_classes < classes[order[i]] ){
	    num_classes = classes[order[i]];
	}
    }
    num_classes++;
    return { classes, num_classes };
}

vector<int> BuildSuffixArray(const string& text) {

    //convert input string to integer and compress their numbers
    vector<int> classes;
    int num_classes = 5;
    for( auto & i : text ){
	int num;
	switch (i) {
	case '$':
	    num = 0;
	    break;
	case 'A':
	    num = 1;
	    break;
	case 'C':
	    num = 2;
	    break;
	case 'G':
	    num = 3;
	    break;
	case 'T':
	    num = 4;
	    break;
	default:
	    assert(false);
	}
	classes.push_back(num);
    }
    
    vector<int> order = stable_sort_char(num_classes, classes);

    // cout << "classes: " << endl;
    // for( auto & i : classes ){
    // 	std::cout << i << " ";
    // }
    // std::cout << endl;
    
    std::tie( classes, num_classes ) = compute_classes( text, order );

    int l = 1;
    while( l < text.size() ){
	order = sort_doubled( text.size(), l, order, classes, num_classes );
	assert(order.size() == text.size());
	auto ret = update_classes( order, classes, l, num_classes );
	classes = ret.first;
	num_classes = ret.second;
    	l = 2*l;
    }

    assert( order.size() == text.size() );
    return order;
}

vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array) {
  vector<int> result;
  
  int min_index = 0;
  int max_index = text.size();
  // cout << "find_min_bound" << endl;
  while( min_index < max_index ){
      int mid_index = (min_index + max_index)/2;
      int idx = suffix_array[mid_index];
      // cout << "compare: " << pattern.c_str() << ", " << text.substr( idx ).c_str() << endl;
      int ret = strncmp( pattern.c_str(), text.substr( idx ).c_str(), pattern.size() );
      if( ret > 0 ){
	  // cout << "stcomp < 0" << endl;
	  // cout << "min_index: " << min_index << endl;
	  min_index = mid_index + 1;
      } else {
	  // cout << "stcomp >= 0" << endl;
	  max_index = mid_index;
	  // cout << "max_index: " << max_index << endl;
      }
  }
  // cout << "find_max_bound" << endl;
  int start = min_index;
  // cout << "start index: " << start << endl;
  max_index = text.size();
  while( min_index < max_index ){
      int mid_index = (min_index + max_index)/2;
      int idx = suffix_array[mid_index];
      // cout << "compare: " << pattern.c_str() << ", " << text.substr( idx ).c_str() << endl;
      int ret = strncmp( pattern.c_str(), text.substr( idx ).c_str(), pattern.size() );
      if( ret < 0 ){
	  // cout << "stcomp < 0" << endl;
	  max_index = mid_index;
	  // cout << "max_index: " << max_index << endl;
      } else {
	  // cout << "stcomp >= 0" << endl;
	  min_index = mid_index + 1;
	  // cout << "min_index: " << min_index << endl;
      }
  }
  int end = max_index;
  // cout << "start, end : " << start << ", " << end << endl;
  for( int i = start; i < end; ++i ){
      result.push_back(suffix_array[i]);
  }
  return result;
}

int main() {
  char buffer[100001];
  scanf("%s", buffer);
  string text = buffer;
  text += '$';
  vector<int> temp = BuildSuffixArray(text);
  text = text.substr(0, text.size()-1);
  vector<int> suffix_array ( temp.begin() + 1, temp.end() );
  // for( auto i = 0; i < suffix_array.size(); ++i ){
  //     cout << suffix_array[i] << " ";
  // }
  // cout << endl;
  int pattern_count;
  scanf("%d", &pattern_count);
  vector<bool> occurs(text.length(), false);
  for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
    scanf("%s", buffer);
    string pattern = buffer;
    vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
    for (int j = 0; j < occurrences.size(); ++j) {
      occurs[occurrences[j]] = true;
    }
  }
  for (int i = 0; i < occurs.size(); ++i) {
    if (occurs[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
