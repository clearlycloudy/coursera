#include <iostream>
#include <algorithm>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;
using std::reverse;

int count = 0;

void dfs(vector<vector<int> > &adj, vector<int> &used, vector<int> &order, int x) {
    //write your code here
    if( used[x] == 0 ){
	used[x] = 1;
	for( auto & i : adj[x] ){
	    dfs( adj, used, order, i );
	}
	order[count] = x;
	count++;
    }
}     

vector<int> toposort(vector<vector<int> > adj) {
    vector<int> used(adj.size(), 0);
    vector<int> order(adj.size());
    //write your code here
    for( int i = 0; i < adj.size(); i++ ){
	dfs( adj, used, order, i );
    }
    std::reverse(order.begin(), order.end());
    return order;
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
    vector<int> order( std::move(toposort(adj)) );
    for (size_t i = 0; i < order.size(); i++) {
	std::cout << order[i] + 1 << " ";
    }
}
