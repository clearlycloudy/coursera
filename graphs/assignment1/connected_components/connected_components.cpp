#include <iostream>
#include <vector>
#include <set>

using std::set;
using std::vector;
using std::pair;

void dfs( vector<vector<int> > & adj, int current, set<int> & reached ){
    reached.insert( current );
    for( auto & i : adj[current] ){
	if( reached.find( i ) == reached.end() ){
	    dfs( adj, i, reached );
	}
    }
}
int number_of_components(vector<vector<int> > & adj) {
    set<int> reached;
    int count = 0;
    for( int i = 0; i < adj.size(); ++i ){
	if( reached.find(i) == reached.end() ){
	    count++;
	    dfs( adj, i, reached );	    
	}
    }
    return count;
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
  std::cout << number_of_components(adj) << std::endl;
}
