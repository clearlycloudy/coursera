#include <iostream>
#include <vector>
#include <set>

using std::vector;
using std::pair;
using std::set;



void dfs( vector<vector<int> > & adj, int current, set<int> & reached ){
    reached.insert( current );
    for( auto & i : adj[current] ){
	if( reached.find( i ) == reached.end() ){
	    dfs( adj, i, reached );
	}
    }
}

int reach(vector<vector<int> > & adj, int x, int y) {    
    set<int> reached;
    dfs( adj, x, reached );
    if( reached.find( y ) == reached.end() ){
	return false;
    }else{
	return true;
    }
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
	int x, y;
	std::cin >> x >> y;
	adj[x - 1].push_back(y - 1);
	adj[y - 1].push_back(x - 1);
    }
    int x, y;
    std::cin >> x >> y;
    std::cout << reach(adj, x - 1, y - 1) << std::endl;
}
