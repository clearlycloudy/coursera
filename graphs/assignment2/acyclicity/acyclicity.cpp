#include <iostream>
#include <vector>
#include <set>

using std::vector;
using std::pair;
using std::set;

bool dfs(vector<vector<int> > &adj, int current, int start, set<int> searched ) {
    if( current == start )
	return true;
    searched.insert(current);
    for( auto & i : adj[current] ){
	if( searched.find(i) == searched.end() ){
	    if( dfs( adj, i, start, searched ) )
		return true;
	}
    }
    return false;
}
int acyclic(vector<vector<int> > &adj) {
    for( int i = 0; i < adj.size(); ++i ){
	set<int> searched;
	for( auto & j : adj[i] ){
	    if( dfs( adj, j, i, searched )  ){
		return 1;
	    }
	}
	
    }
    return 0;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
	int x, y;
	std::cin >> x >> y;
	adj[x - 1].push_back(y - 1);
    }
    std::cout << acyclic(adj) << std::endl;
}
