#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Node {
    int key;
    int left;
    int right;
    bool visited;
    int lim_l;
    int lim_h;
    Node() : key(0), left(-1), right(-1), visited(false),
	     lim_l( std::numeric_limits<int>::min() ),
	     lim_h( std::numeric_limits<int>::max() ){}
    Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_), visited(false),
					    lim_l( std::numeric_limits<int>::min() ),
					    lim_h( std::numeric_limits<int>::max() ){}
};

bool check_node_child( int idx, vector<Node>& tree ){
    int k = tree[idx].key;
    int l = tree[idx].left;
    int r = tree[idx].right;
    // cout << "key: " << k << ", lim: " << tree[idx].lim_l << ", " << tree[idx].lim_h << endl;
    if( l != -1 && tree[l].key >= k ){
	return false;
    }
    if( r != -1 && tree[r].key <= k ){
	return false;
    }
    if( tree[idx].key >= tree[idx].lim_h ||
	tree[idx].key <= tree[idx].lim_l ){
	return false;
    }
    return true;
}
bool IsBinarySearchTree(vector<Node>& tree) {
    if( tree.empty() )
	return true;
    vector<int> q {};
    q.push_back( 0 );//root
    while( !q.empty() ){
	int idx_current = q.back();
	Node i = tree[idx_current];
	q.pop_back();
	if( i.visited ){
	    continue;
	}
	if( tree[idx_current].left != -1 ){
	    tree[tree[idx_current].left].lim_h = tree[idx_current].key;
	    tree[tree[idx_current].left].lim_l = tree[idx_current].lim_l;
	}
	if( tree[idx_current].right != -1 ){
	    tree[tree[idx_current].right].lim_h = tree[idx_current].lim_h;
	    tree[tree[idx_current].right].lim_l = tree[idx_current].key;
	}
	if( !check_node_child( idx_current, tree ) ){
	    return false;
	}
	if( tree[idx_current].left != -1 )
	    q.push_back( tree[idx_current].left );
	if( tree[idx_current].right != -1 )
	    q.push_back( tree[idx_current].right );
    }
    return true;
}

int main() {
    int nodes;
    cin >> nodes;
    vector<Node> tree;
    for (int i = 0; i < nodes; ++i) {
	int key, left, right;
	cin >> key >> left >> right;
	tree.push_back(Node(key, left, right));
    }
    if (IsBinarySearchTree(tree)) {
	cout << "CORRECT" << endl;
    } else {
	cout << "INCORRECT" << endl;
    }
    return 0;
}
