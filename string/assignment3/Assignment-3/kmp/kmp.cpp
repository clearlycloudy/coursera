#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

vector<int> compute_prefix( const string & in ) {
    vector<int> v( in.size() );
    int idx_border = 0;
    v[0] = 0;
    for( int i = 1; i < in.size(); ++i ){
	while( idx_border > 0 &&
	       in[idx_border] != in[i] ){
	    //decrease border to previous match index and check match again
	    idx_border = v[idx_border-1];
	}
	if( in[i] == in[idx_border] ){
	    // std::cout << "match: " << i << std::endl;
	    idx_border++;
	} else {
	    idx_border = 0;
	}
	v[i] = idx_border;
    }
    return v;
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where 
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {

    string s = pattern + "$" + text;
    
    vector<int> prefix = compute_prefix( s );

    // for( auto i : prefix ){
    // 	std::cout << i << " ";
    // }
    // std::cout << std::endl;
    
    vector<int> result;

    //start searching in text that contains prefix length of |pattern|
    for( int i = pattern.size() + 1; i < prefix.size(); ++i ){
	// std::cout << "size: " << prefix[i] << std::endl;
	if( prefix[i] == pattern.size() ){
	    result.push_back( i - 2 * pattern.size() );
	}
    }
  
    return result;
}

int main() {
    string pattern, text;
    cin >> pattern;
    cin >> text;
    vector<int> result = find_pattern(pattern, text);
    for (int i = 0; i < result.size(); ++i) {
	printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}
