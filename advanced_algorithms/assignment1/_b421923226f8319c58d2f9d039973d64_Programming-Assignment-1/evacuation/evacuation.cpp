#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <tuple>
#include <set>
#include <utility>

using std::set;
using std::map;
using std::vector;
using std::pair;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */

struct Edge {
    int from, to, capacity, flow;
};

class FlowGraph {
public:
private:
    /* List of all - forward and backward - edges */
    vector<Edge> edges;

    /* These adjacency lists store only indices of edges in the edges list */
    vector<vector<size_t> > graph;
public:
    map<pair<int,int>, Edge * > edge_map;

public:
    explicit FlowGraph(size_t n, size_t m): graph(n) {
	edges.reserve(m*2);
    }

    void add_edge(int from, int to, int capacity) {
        /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
	if( from == to ){
	    capacity = 0;
	}
	auto it_exist = edge_map.find({from,to});
	if( it_exist != edge_map.end() ){
	    it_exist->second->capacity += capacity;
	    edge_map[{to,from}]->capacity += capacity;
	    edge_map[{to,from}]->flow += capacity;
	}else{
	    Edge forward_edge = {from, to, capacity, 0};
	    Edge backward_edge = {to, from, capacity, capacity};
	    graph[from].push_back(edges.size());
	    edges.push_back(forward_edge);
	    edge_map[{from,to}] = &edges.back();
	    graph[to].push_back(edges.size());
	    edges.push_back(backward_edge);
	    edge_map[{to,from}] = &edges.back();
	}
    }

    size_t size() const {
        return graph.size();
    }

    const vector<size_t>& get_ids(int from) const {
        return graph[from];
    }

    Edge * get_edge(size_t id){
        return &edges[id];
    }

    void add_flow(size_t id, int flow) {
        /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }
};

FlowGraph read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    FlowGraph graph(vertex_count, edge_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
    }
    return graph;
}

int max_flow(FlowGraph& graph, int from, int to) {
    int flow = 0;

    while(true){
	vector<int> q;
	q.push_back( 0 );
        map<int,Edge*> path {};
	while( !q.empty() ){
	    int current = q.back();
	    q.pop_back();
	    auto edge_idxs = graph.get_ids( current );
	    for( auto i : edge_idxs ){
	        Edge * e = graph.get_edge( i );
		if( path.find( e->to ) == path.end() &&
		    e->to != from &&
		    e->capacity > e->flow ){
		    // std::cout << "e: " << e->from << "->" << e->to << ", flow: " << e->flow << ", cap: " << e->capacity << std::endl;
		    path[e->to] = e;
		    q.push_back(e->to);
		}
	    }
	}
	if( path.find( to ) == path.end() ){
	    break;
	} else {
	    //found a augmenting path
	    // set<int> travelled;
	    auto d = std::numeric_limits<int>::max();
	    Edge * e = path[to];
	    auto it = path.find( e->to );
	    //find the edge having smallest additional flow
	    while( it != path.end() ){
		// bool inserted;
		// std::tie(std::ignore, inserted ) = travelled.insert(it->second->from);
		// if( !inserted ){
		//     break;
		// }
		// std::cout << "path from: " << it->second->from << std::endl;
		// std::cout << "path to: " << it->second->to << std::endl;
		d = std::min( d, it->second->capacity - it->second->flow );
		it = path.find( it->second->from );
	    }
	    //d is now the maximum amount that can be propagated in a path
	    //update the flow in that path
	    it = path.find( e->to );
	    // set<int> travelled2;
	    while( it != path.end() ){
		// bool inserted;
		// std::tie(std::ignore, inserted ) = travelled2.insert(it->second->from);
		// if( !inserted ){
		//     break;
		// }
		// std::cout << "vert: " << it->second->from <<" - " << it->second->to << std::endl;
	        Edge * edge_forward = graph.edge_map[{it->second->from, it->second->to}];
		Edge * edge_back = graph.edge_map[{it->second->to, it->second->from}];
		edge_forward->flow += d;
	        edge_back->flow -= d;
		// std::cout << "post_adjust: " << edge_forward->from << "->" << edge_forward->to << ", flow: " << edge_forward->flow << ", cap: " << edge_forward->capacity << std::endl;
		it = path.find( it->second->from );
	    }
	    flow += d;
	    // std::cout << "flow: " << flow << std::endl;
	}
    }
    return flow;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    std::cout << max_flow(graph, 0, graph.size() - 1) << "\n";
    return 0;
}
