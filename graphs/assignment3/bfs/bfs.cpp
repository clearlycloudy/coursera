#include <iostream>
#include <vector>
#include <queue>

using namespace std;


int bfs( vector<vector<int>> & adj, int src, int dest ){
    queue<int> q;
    vector<bool> visited( adj.size(), false );
    vector<int> distances( adj.size() );
    distances[src] = 0;
    q.push( src );
    while( !q.empty() ){
	int current = q.front();
	q.pop();
	//queue neighbours
	for( auto & i : adj[current] ){
	    //only visit unexplored node
	    if( !visited[i] ){
		//update distance for newly found node
		distances[i] = distances[current] + 1;
		q.push( i );
		//mark as finished
		visited[i] = true;
	    }	    
	}
    }
    if( visited[dest] ){
	return distances[dest];
    }else{
	return -1;
    }
}

int distance(vector<vector<int> > & adj, int s, int t) {
    return bfs( adj, s, t );
}

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (int i = 0; i < m; i++) {
	int x, y;
	std::cin >> x >> y;
	adj[x - 1].push_back(y - 1);
	adj[y - 1].push_back(x - 1);
    }
    int s, t;
    std::cin >> s >> t;
    s--, t--;
    std::cout << distance(adj, s, t);
}
