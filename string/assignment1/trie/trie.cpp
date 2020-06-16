#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
    trie t {};
    t.push_back( {} );
    for( auto & i : patterns ){
	int current_node = 0;
	for( auto & c: i ){
	    if( t[current_node].find(c) != t[current_node].end() ){
		//existing node
		current_node = t[current_node][c];
	    }else{
		//add new node
		t.push_back({});
		int new_node = t.size()-1;
		t[current_node][c] = new_node;
		current_node = new_node;
	    }
	}
    }
    return t;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<string> patterns;
    for (size_t i = 0; i < n; i++) {
	string s;
	std::cin >> s;
	patterns.push_back(s);
    }

    trie t = build_trie(patterns);
    for (size_t i = 0; i < t.size(); ++i) {
	for (const auto & j : t[i]) {
	    std::cout << i << "->" << j.second << ":" << j.first << "\n";
	}
    }

    return 0;
}
