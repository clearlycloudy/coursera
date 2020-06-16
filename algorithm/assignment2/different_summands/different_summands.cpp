#include <iostream>
#include <vector>

using namespace std;

void aux( int l, int n, vector<int> & accum ){
    // cout << "n: " << n << " ,l: " << l << endl;
    if( 2*l >= n ){
	accum.push_back(n);
	// cout << "Ret" << endl;
	return;
    }else{
	// cout << "push: " << l << endl;
	accum.push_back(l);
	return aux( l + 1, n - l, accum );
    }
}

vector<int> optimal_summands(int n) {
    vector<int> summands {};

    aux( 1, n, summands );
    
    return summands;
}


vector<int> optimal_summands_2( int n ){
    vector<int> summands {};
    int sum = 0;
    for( int i = 1; i <= n; ++i ){
	if( n - sum <= 2*(i+1) ){
	    if( n-sum > 0 ){
		summands.push_back(n-sum);
	    }
	    break;
	}
	sum += i;
	summands.push_back(i);
    }
    return summands;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> summands = optimal_summands(n);
    std::cout << summands.size() << '\n';
    for (size_t i = 0; i < summands.size(); ++i) {
	std::cout << summands[i] << ' ';
    }
    cout << endl;
}
