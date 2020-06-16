#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>

using namespace std;

void find_moves_b( int n, int total, vector<int> & mem ){
    while( n <= total ){
	//conisder all possibilities
	vector<int> options { std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
	if( n%3 == 0 ){
	    options[0] = mem[n/3] + 1;
	}
	if( n%2 == 0 ){
	    options[1] = mem[n/2] + 1;
	}
	options[2] = mem[n-1] + 1;

	//pick an option of lowest cost
	mem[n] = *std::min_element(options.begin(),options.end());
	
	n++;
    }
}

void optimal_sequence(int n, vector<int> & mem) {
    std::vector<int> sequence;
    find_moves_b( 2, n, mem );
    //backtrack
    int i = n;
    while( i >= 1 ){
    	auto a = std::numeric_limits<int>::max();
    	auto b = std::numeric_limits<int>::max();
    	if( i%3 == 0 )
    	    a = mem[i/3];
    	if( i%2 == 0 )
    	    b = mem[i/2];	
    	auto c = mem[i-1];
    	vector<int > options {a,b,c};
    	auto it = std::min_element( options.begin(), options.end());
    	sequence.push_back(i);
    	if( it-options.begin() == 0 )
    	    i = i/3;
    	else if( it-options.begin() == 1 )
    	    i = i/2;
    	else
    	    i = i-1;
    }
    reverse(sequence.begin(), sequence.end());
    cout << sequence.size() - 1 << endl;
    for( auto & j : sequence ){
    	cout << j << " ";
    }
    cout << endl;
    return;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> mem(n+1, std::numeric_limits<int>::max() );
    mem[1] = 0;
    mem[0] = 0;
    optimal_sequence(n, mem);
}
