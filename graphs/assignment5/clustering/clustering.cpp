#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>
#include <set>
#include <queue>
#include <limits>

using namespace std;

struct node {
    int id;
    int x;
    int y;
    int group_id;
};

struct edge {
    int id_a;
    int id_b;
    double dist;
};

class comp {
public:
    bool operator()( edge const & a, edge const & b ){
	return a.dist > b.dist;
    }
};

double calc_dist( int x1, int y1, int x2, int y2 ){
    return sqrt( pow(((double)x1-x2),2.0) + pow(((double)y1-y2),2.0) );
}

double clustering(vector<int> x, vector<int> y, int k) {
    double dist = -1;
    vector<node> nodes( x.size() );
    set<int> group_count{};
    priority_queue<edge, vector<edge>, comp> q {};

    //initialize nodes
    for( int i = 0; i < x.size(); ++i ){
	nodes[i] = { i, x[i], y[i], i };
	group_count.insert(i);
    }
    //compute all edge distances
    for( int i = 0; i < x.size(); ++i ){
	for( int j = 0; j < x.size(); ++j ){
	    if( i != j ){
		edge e1 { i, j, calc_dist( nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y ) };
		edge e2 { j, i, calc_dist( nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y ) };
		q.push(e1);
		q.push(e2);
	    }
	}
    }
    //run kruskal algorithm with added cluster counter
    while( !q.empty() && group_count.size() > 0 ){
	edge e = q.top();
	q.pop();
        if( nodes[e.id_a].group_id != nodes[e.id_b].group_id ){
	    int temp_group_id = nodes[e.id_b].group_id;
	    int temp_group_id_dest = nodes[e.id_a].group_id;
	    //make the group id same
	    for( auto & i : nodes ){
		if( i.group_id == temp_group_id ){
		    i.group_id = temp_group_id_dest;
		}
	    }
	    if( group_count.size() == k ){
		//record minimal distance between k clusters
		dist = e.dist;
		break;
	    }
	    group_count.erase( temp_group_id );
	}
    }
    
    return dist;
} 

int main() {
    size_t n;
    int k;
    std::cin >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> x[i] >> y[i];
    }
    std::cin >> k;
    std::cout << std::setprecision(10) << clustering(x, y, k) << std::endl;
}
