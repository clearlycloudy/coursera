#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<long long> &distance, vector<int> &reachable, vector<int> &shortest) {
    vector< long long > dist( adj.size(), std::numeric_limits<long long>::max() );
    dist[s] = 0;
    //bellman-ford-moore algorithm
    size_t verts = adj.size();
    for( int i = 1; i <= verts-1; ++i ){ //V-1 iterations
	for( int j = 0; j < adj.size(); ++j ){ //relax edges
	    for( int k = 0; k < adj[j].size(); ++k ){
		int start = j;
		int end = adj[j][k];
		if( dist[start] != std::numeric_limits<long long>::max() &&
		    dist[end] > dist[start] + cost[j][k] ){
		    dist[end] = dist[start] + cost[j][k];
		    // cout << "relaxed: " << start+1 << ", " << end+1 << ": " << dist[end] << endl;
		}
	    }
	}
    }
    //negative cycle detection
    queue<int> q;
    for( int j = 0; j < adj.size(); ++j ){ //relax edges
	for( int k = 0; k < adj[j].size(); ++k ){
	    int start = j;
	    int end = adj[j][k];
	    if( dist[start] != std::numeric_limits<long long>::max() &&
		dist[end] > dist[start] + cost[j][k] ){
		dist[end] = dist[start] + cost[j][k];
		q.push( end );
		// cout << "negative edge: " << start+1 << ", " << end+1 << ": " << dist[end] << endl;
	    }
	}
    }
    //find all nodes in the negative cycle
    vector<bool> visited( adj.size(), false );
    while( !q.empty() ){
	int current = q.front();
	// cout << "negative cycle node: " << current+1 << endl;
	q.pop();
	if( visited[current] == true )
	    continue;
	for( auto & i : adj[current] ){
	    q.push(i);
	}
	visited[current] = true;
    }

    //save result
    for( int i = 0; i < adj.size(); ++i ){
	if( dist[i] == std::numeric_limits<long long>::max() ){ //not reachable
	    reachable[i] = 0;
	}
	else if( visited[i] ){ //negative cycle
	    shortest[i] = 0;
	    reachable[i] = 1;
	}else{ //reachable non-negative cycle
	    distance[i] = dist[i];
	    reachable[i] = 1;
	}
    }
}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<long long> distance(n, std::numeric_limits<long long>::max());
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
