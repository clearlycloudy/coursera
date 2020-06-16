#include <algorithm>
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;

void explore( int i, vector<vector<int>> & adj, vector<int> & components, int componentcount ){
    if( components[i] == -1 ){
	components[i] = componentcount;
	for( auto & j : adj[i] ){
	    explore( j, adj, components, componentcount );
	}
    }
}

void dfs( int i, vector<vector<int>> & adj, vector<int> & visited, vector<int> & order ){
    if( visited[i] == 0 ){
	visited[i] = 1;
	for( auto & j : adj[i] ){
	    dfs( j, adj, visited, order );
	}
	order.push_back(i);
    }
}

int number_of_strongly_connected_components(vector<vector<int> > & adj ) {
    vector<int> order;
    vector<int> visited( adj.size(), 0 );
    for( int i = 0; i < adj.size(); i++ ){
	dfs( i, adj, visited, order );
    }
    std::reverse( order.begin(), order.end() ); //post order
    // std::cout << "size: " << order.size() << std::endl;
    vector<int> components( adj.size(), -1 );
    int componentcount = 0;

    //reverse graph
    vector<vector<int>> adj_rev( adj.size(), vector<int>());
    for( int i = 0; i < adj.size(); i++ ){
	for( auto & j : adj[i] ){
	    adj_rev[j].push_back(i);
	}
    }
    
    for( auto & i : order ){
	if( components[i] == -1 ){   
	    componentcount++;
	    explore( i, adj_rev, components, componentcount );
	}
    }
    return *std::max_element(components.begin(), components.end());
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
    std::cout << number_of_strongly_connected_components(adj) << std::endl;
}
