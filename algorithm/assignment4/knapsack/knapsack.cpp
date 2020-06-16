#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int optimal_weight(int W, const vector<int> & w, vector<int> & backtrack ) {
    //memoization
    vector<vector<int>> mem( w.size()+1, vector<int>(W+1,0) );
    //base conditions
    for (size_t i = 0; i <= w.size(); i++) {
	mem[i][0] = 0;
    }
    for (size_t i = 0; i <= W; i++) {
	mem[0][i] = 0;
    }
    for (size_t i = 1; i <= w.size(); i++) {
	for (size_t j = 1; j <= W; j++) {
	    //exclude item
	    mem[i][j] = mem[i-1][j];

	    //try include item
	    if( w[i] <= j ){
		int val = mem[i-1][j-w[i]] + w[i]; //use previous computation
		if( val > mem[i][j] ){ //check constraints
		    mem[i][j] = val; //include item
		}
	    }
	}
    }

    // //backtrack
    // int j = W;
    // for( int i = w.size()-1; i >= 1; i-- ){
    // 	int val = mem[i-1][j]; //exlude
    // 	int val2 = mem[i-1][j-w[i]] + w[i]; //include
    // 	if( val2 > val ){
    // 	    j = j-w[i];
    // 	    backtrack.push_back(i);
    // 	}
    // }
    
    return mem[w.size()-1][W];
}

int main() {
    int n, W;
    std::cin >> W >> n;
    vector<int> w(n+1);
    w[0] = 0;
    for (int i = 0; i < n; i++) {
	std::cin >> w[i+1];
    }
    vector<int> backtrack;
    std::cout << optimal_weight(W, w, backtrack) << '\n';

    // std::cout << "backtrack: ";
    // for( auto & i : backtrack ){
    // 	cout << w[i] << " ";
    // }
    // cout << endl;
}
