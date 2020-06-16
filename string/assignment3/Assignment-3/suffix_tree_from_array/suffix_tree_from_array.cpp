#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::vector;
using namespace std;

struct Node {
    int parent;
    map<char, int> children;
    int depth;
    int edge_start;
    int edge_end;
};

// Data structure to store edges of a suffix tree.
struct Edge {
    // The ending node of this edge.
    int node;
    // Starting position of the substring of the text 
    // corresponding to the label of this edge.
    int start;
    // Position right after the end of the substring of the text 
    // corresponding to the label of this edge.
    int end;

    Edge(int node_, int start_, int end_ ) : node(node_), start(start_), end(end_) {}

    Edge(const Edge& e) : node(e.node), start(e.start), end(e.end) {}
};

// Build suffix tree of the string text given its suffix array suffix_array
// and LCP array lcp_array. Return the tree as a mapping from a node ID
// to the vector of all outgoing edges of the corresponding node. The edges in the
// vector must be sorted in the ascending order by the first character of the edge label.
// Root must have node ID = 0, and all other node IDs must be different
// nonnegative integers.
//
// For example, if text = "ACACAA$", an edge with label "$" from root to a node with ID 1
// must be represented by Edge(1, 6, 7). This edge must be present in the vector tree[0]
// (corresponding to the root node), and it should be the first edge in the vector 
// (because it has the smallest first character of all edges outgoing from the root).

map<int, vector<Edge> > SuffixTreeFromSuffixArray(
    const vector<int>& suffix_array,
    const vector<int>& lcp_array,
    const string& text) {

    vector<int> suffix_array_inverse_map( suffix_array.size() );
    for( int i = 0; i < suffix_array.size(); ++i ){
	suffix_array_inverse_map[ suffix_array[i] ]  = i;
    }

    struct Node {
    int parent;
    map<char, int> children;
    int depth;
    int edge_start;
    int edge_end;
};
    
    vector<Node> nodes;
    Node root;
    root.parent = -1;
    root.depth = 0;
    root.edge_start = -1;
    root.edge_end = -1;
    nodes.push_back(root);
    
    int lcp_prev = 0;
    int cur_node = 0;

    map<int, vector<Edge> > tree;
    for( int i = 0; i < suffix_array.size(); ++i ){
	tree[i] = {};
    }

    for( int i = 0; i < text.size(); ++i ){
	int suffix = suffix_array[i];
	while( nodes[cur_node].depth > lcp_prev ){
	    cur_node = nodes[cur_node].parent;
	}
	if( nodes[cur_node].depth == lcp_prev ){
	    //create new leaf
	    Node leaf;
	    leaf.parent = cur_node;
	    leaf.depth = text.size() - suffix;
	    leaf.edge_start = suffix + nodes[cur_node].depth;
	    leaf.edge_end = text.size()-1;
	    int node_id = nodes.size();
	    nodes.push_back( leaf );
	    nodes[cur_node].children[text[leaf.edge_start]] = node_id;
	    cur_node = node_id;
	} else {
	    int edgestart = suffix_array[i-1] + nodes[cur_node].depth;
	    int offset = lcp_prev - nodes[cur_node].depth;
	    //break edge
	    char startchar = text[edgestart];
	    char midchar = text[edgestart + offset];
	    int midnode;
	    {
		int node_id = nodes.size();
		Node node_mid;
		node_mid.parent = cur_node;
		node_mid.depth = nodes[cur_node].depth + offset;
		node_mid.edge_start = edgestart;
		node_mid.edge_end = edgestart + offset - 1;
		node_mid.children[midchar] = nodes[cur_node].children[startchar];
		nodes[nodes[cur_node].children[startchar]].parent = node_id;
		nodes[nodes[cur_node].children[startchar]].edge_start = edgestart + offset;
		nodes[cur_node].children[startchar] = node_id;
		nodes.push_back( node_mid );
		midnode = node_id;
	    }
	    //new node
	    Node leaf;
	    leaf.parent = midnode;
	    leaf.depth = text.size() - suffix;
	    leaf.edge_start = suffix + nodes[midnode].depth;
	    leaf.edge_end = text.size()-1;
	    int node_id = nodes.size();
	    nodes.push_back( leaf );
	    nodes[midnode].children[text[leaf.edge_start]] = node_id;
	    cur_node = node_id;
	}
	if( i < text.size() - 1 ){
	    lcp_prev = lcp_array[i];
	}
    }

    for(int i = 1; i < nodes.size(); ++i ){
	// int i = idx;
	// cout << i << ": parent: " << nodes[idx].parent << ", depth: " << nodes[idx].depth << ", edge_start: " << nodes[idx].edge_start << ", edge_end: " << nodes[idx].edge_end << ", children: ";
	// for( auto & j : nodes[idx].children ){
	//     cout << "( " << j.first << ": " << j.second << " )" << ", ";
	// }
	// cout << endl;
	Edge e( i, nodes[i].edge_start, nodes[i].edge_end+1 );
	tree[nodes[i].parent].push_back( e );
	// for( auto & j : nodes[i].children ){


	// }
    }

    // cout << "tree: " << endl;
    // for( auto & i : tree ){
    // 	for( auto & j : i.second ){
    // 	    cout << i.first << ": " << j.node << ": " << j.start << ", " << j.end << endl;
    // 	}
    // }
    
    return tree;
}


int main() {
    char buffer[200001];
    scanf("%s", buffer);
    string text = buffer;

    // cout << "text: " << text << endl;
  
    // cout << "suffix_array" << endl;
    
    vector<int> suffix_array(text.length());
    for (int i = 0; i < text.length(); ++i) {
	scanf("%d", &suffix_array[i]);
	// cout << suffix_array[i] << " ";
    }
    // cout << endl;

    // cout << "lcp_array" << endl;

    vector<int> lcp_array(text.length() - 1);
    for (int i = 0; i + 1 < text.length(); ++i) {
	scanf("%d", &lcp_array[i]);
	// cout << lcp_array[i] << " ";
    }
    // cout << endl;

    // Build the suffix tree and get a mapping from 
    // suffix tree node ID to the list of outgoing Edges.
    map<int, vector<Edge> > tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
    printf("%s\n", buffer);
    // Output the edges of the suffix tree in the required order.
    // Note that we use here the contract that the root of the tree
    // will have node ID = 0 and that each vector of outgoing edges
    // will be sorted by the first character of the corresponding edge label.
    //
    // The following code avoids recursion to avoid stack overflow issues.
    // It uses a stack to convert recursive function to a while loop.
    // The stack stores pairs (node, edge_index). 
    // This code is an equivalent of 
    //
    //    OutputEdges(tree, 0);
    //
    // for the following _recursive_ function OutputEdges:
    //
    // void OutputEdges(map<int, vector<Edge> > tree, int node_id) {
    //   const vector<Edge>& edges = tree[node_id];
    //   for (int edge_index = 0; edge_index < edges.size(); ++edge_index) {
    //     printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
    //     OutputEdges(tree, edges[edge_index].node);
    //   }
    // }
    //

    // vector<pair<int, int> > stack(1, make_pair(0, 0));    
    // while (!stack.empty()) {
    // 	pair<int, int> p = stack.back();
    // 	stack.pop_back();
    // 	int node = p.first;
    // 	int edge_index = p.second;
    // 	if (!tree.count(node)) {
    // 	    continue;
    // 	}
    // 	const vector<Edge>& edges = tree[node];
    // 	if (edge_index + 1 < edges.size()) {
    // 	    stack.push_back(make_pair(node, edge_index + 1));
    // 	}
    // 	printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
    // 	stack.push_back(make_pair(edges[edge_index].node, 0));
    // }

    for( auto & i : tree ){
	for( auto & j : i.second ){
	    cout << j.start << " " << j.end << endl;
	}
    }

    return 0;
}
