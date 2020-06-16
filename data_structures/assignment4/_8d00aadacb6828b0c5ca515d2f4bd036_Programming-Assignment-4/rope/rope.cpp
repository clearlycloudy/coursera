#include <cstdio>
#include <string>
#include <iostream>


class Rope {
    std::string s;
public:
    Rope(const std::string &s) : s(s) { 
    }

    void process( int i, int j, int k ) {
	// Replace this code with a faster implementation
	std::string t = s.substr(0, i) + s.substr(j + 1);
	s = t.substr(0, k) + s.substr(i, j - i + 1) + t.substr(k);
    }

    std::string result() {
	return s;
    }
};

node * root;

class node {
public:
    int count;
    int start;
    int end;
    bool leaf;
    node * left;
    node * right;
};

node * find_node( node * n, int key ){
    // if(!n){
    // 	return 0;
    // }
    // if( n->leaf ){
    // 	return n;
    // }
    // node * last = 0;
    // while( true ){	
    // 	if( key >= n->count ){
    // 	    last = n;
    // 	    int sum = 0;
    // 	    if( n->right->leaf ){
    // 		sum = n->right->count + n->count;
    // 	    }
    // 	    n = n->right;
    // 	}
    // 	else{
    // 	    n = n->left;
    // 	}
    // }
    return 0;
}

void process_rope( int i, int j, int k ){
    
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::string s;
    std::cin >> s;
    Rope rope(s);
    int actions;
    std::cin >> actions;

    root = new node;
    root->count = s.size();
    root->leaf = true;
    root->left = 0;
    root->right = 0;
    root->start = 0;
    root->end = s.size();
    
    for (int action_index = 0; action_index < actions; ++action_index) {
	int i, j, k;
	std::cin >> i >> j >> k;
	rope.process(i, j, k);
    }
    std::cout << rope.result() << std::endl;

    //cleanup
    vec<node*> q;
    q.push_back(root);
    while(!q.empty()){
	Node * n = q.back();
	q.pop_back();
	if(n){
	    q.push_back( n->left );
	    q.push_back( n->right );
	    delete n;
	}
    }
}
