#include <iostream>

#include <vector>
#include <queue>
#include <limits>
#include <functional>

using namespace std;

class comp {
public:
    bool operator()( pair<int,unsigned int> const & a, pair<int,unsigned int> const & b ){
	if( a.second > b.second )
	    return true;
	else
	    return false;
    }
};

int distance( vector<vector<int> > & adj, vector<vector<int> > & cost, int s, int t ) {
    vector<bool> visited( adj.size(), false );
    vector<unsigned int> dist( adj.size(), std::numeric_limits<unsigned int>::max() );
    priority_queue<pair<int,unsigned int>, vector<pair<int,unsigned int>>, comp > q;
    pair<int,unsigned int> source = pair<int, unsigned int>(s,0);
    dist[s] = 0;
    q.push(source);
    while( !q.empty() ){
	pair<int, unsigned int> current_node = q.top();
	int current = current_node.first;
	q.pop();
	if( visited[current] ){
	    continue;
	}
	for( int i = 0; i < adj[current].size(); ++i ){
	    int neighbour = adj[current][i];
	    if( cost[current][i] + dist[current] < dist[neighbour] ){
		//relax weight of edge
		dist[neighbour] = cost[current][i] + dist[current];
		pair<int,unsigned int> updated_node { neighbour, dist[neighbour] };
		//enqueue updated edge
		q.push( updated_node );
	    }
	}
	visited[current] = true;
    }
    if( visited[t] == false ){
	return -1;
    }else{
	return dist[t];
    }
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
    int s, t;
    std::cin >> s >> t;
    s--, t--;
    std::cout << distance(adj, cost, s, t);
}
