#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
    // for( int current = 0; current < adj.size(); ++current ){
	vector< int > dist( adj.size(), 0 );
	//bellman-ford-moore algorithm
	size_t verts = adj.size();
	for( int i = 1; i <= verts-1; ++i ){ //V-1 iterations
	    for( int j = 0; j < adj.size(); ++j ){ //relax edges
		for( int k = 0; k < adj[j].size(); ++k ){
		    int start = j;
		    int end = adj[j][k];
		    if( dist[start] != std::numeric_limits<int>::max() &&
			dist[end] > dist[start] + cost[j][k] ){
			dist[end] = dist[start] + cost[j][k];
			// cout << "relaxed: " << start+1 << ", " << end+1 << ": " << dist[end] << endl;
		    }
		}
	    }
	}
	//negative cycle detection
	for( int j = 0; j < adj.size(); ++j ){ //relax edges
	    for( int k = 0; k < adj[j].size(); ++k ){
		int start = j;
		int end = adj[j][k];
		if( dist[start] != std::numeric_limits<int>::max() &&
		    dist[end] > dist[start] + cost[j][k] ){
		    return 1;
		}
	    }
	}
    // }
    return 0;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    for (int i = 0; i < m; i++) {
	int x, y, w;
	std::cin >> x >> y >> w;
	adj[x - 1].push_back(y - 1);
	cost[x - 1].push_back(w);
    }
    std::cout << negative_cycle(adj, cost) << endl;
}
