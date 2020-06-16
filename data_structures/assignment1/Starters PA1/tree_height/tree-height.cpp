#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct node {
    node() : _count(0), children{} {}
    int _count;
    vector<node *> children;
    void visit( int count, int & max ){
	_count = count + 1;
	if( _count > max ){
	    max = _count;
	}
	for( auto & i : children ){
	    i->visit( _count, max );
	}
    }
};

class TreeHeight {
    int n;             
    vector<node> nodes;
    int root;
public:
    void read() {
	std::cin >> n;
	nodes.resize(n);
	for (int i = 0; i < n; i++){
	    int val;
	    std::cin >> val;
	    if( val != -1 ){
		nodes[val].children.push_back(&nodes[i]);
	    }else{
		root = i;
	    }
	}
    }
    int compute_height() {
	// Replace this code with a faster implementation
	int maxHeight = 0;
	nodes[root].visit(0,maxHeight);
	return maxHeight;
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    TreeHeight tree;
    tree.read();
    std::cout << tree.compute_height() << std::endl;
}
