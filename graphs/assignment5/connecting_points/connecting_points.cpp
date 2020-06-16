#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <queue>
#include <set>
#include <limits>

using namespace std;

struct node {
    int id;
    int x;
    int y;
    double dist;
};

class comp {
public:
    bool operator()( node const & a, node const & b ){
	return a.dist > b.dist;
    }
};

double compute_dist( int x1, int y1, int x2, int y2 ){
    return sqrt( pow((x1-x2),2.0) + pow((y1-y2),2.0) );
}

double minimum_distance_prim(vector<int> x, vector<int> y) {

    //prim algorithm
    priority_queue<node,vector<node>, comp > q;
    size_t count = x.size();
    vector<node> nodes{};
    for( int i = 0; i < count; ++i ){
	nodes.push_back( { i, x[i], y[i], std::numeric_limits<int>::max() } );
    }
    set<int> found{};
    nodes[0].dist = 0;
    q.push(nodes[0]);
    
    double dist = 0;
    
    while( !q.empty() ){
	node n = q.top();
	q.pop();
	if( found.find(n.id) == found.end() ){
	    found.insert(n.id);
	    dist += n.dist;
	    //consider distances to other nodes
	    for( size_t i = 0; i < count; ++i ){
		if( found.find(nodes[i].id) == found.end() ){
		    double d = compute_dist( n.x, n.y, nodes[i].x, nodes[i].y );
		    // cout << i << ":" << d << endl;
		    nodes[i].dist = d;
		    q.push(nodes[i]);
		}
	    }
	}
    }
    
    return dist;
} 

int main() {
    size_t n;
    std::cin >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++) {
	std::cin >> x[i] >> y[i];
    }
    std::cout << std::setprecision(10) << minimum_distance_prim(x, y) << std::endl;
}
