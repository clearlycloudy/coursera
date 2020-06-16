#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

int bipartite( vector<vector<int> > & adj ){    
    queue<int> q;
    vector<bool> visited( adj.size(), false );
    vector<int> dist( adj.size(), -1 );
    q.push(0);
    dist[0] = 0;
    while( !q.empty() ){
	int current = q.front();
	q.pop();
	for( auto & i : adj[current] ){
	    if( visited[i] ){
		if( abs(dist[i] - dist[current]) % 2 == 0 ){
		    //non-bipartite condition satisfied
		    return 0;
		}
	    }else{
                //queue newly found node
		visited[i] = true;
		dist[i] = dist[current]+1;
		q.push(i);
	    }
	}
    }
    return 1;
}

int main(){
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (int i = 0; i < m; i++) {
	int x, y;
	std::cin >> x >> y;
	adj[x - 1].push_back(y - 1);
	adj[y - 1].push_back(x - 1);
    }
    std::cout << bipartite(adj);
}
