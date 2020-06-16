#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::vector;
using std::ios_base;
using std::cin;
using std::cout;

class TreeOrders {
    int n;
    vector <int> key;
    vector <int> left;
    vector <int> right;

public:
    void read() {
	cin >> n;
	key.resize(n);
	left.resize(n);
	right.resize(n);
	for (int i = 0; i < n; i++) {
	    cin >> key[i] >> left[i] >> right[i];
	}
    }

    vector <int> in_order() {
        vector<int> k;
        vector<int> q;
	
	q.push_back(0);

	bool ignore_left = false;
	while( k.size() < key.size() ){
	    int current_idx = q.back();
	    int l = left[current_idx];
	    int r = right[current_idx];
	    if( l != -1 && ignore_left == false ){
		q.push_back(l);
	    }else{
		ignore_left = false;
		k.push_back(current_idx);
		q.pop_back();
		if( r != -1 ){
		    q.push_back(r);
		}else{
		    ignore_left = true;
		}
	    }
	}
	for(auto & i : k ){
	    i = key[i];
	}
	return k;
    }

    vector <int> pre_order() {
        vector<int> k;
        vector<int> q;
	
	q.push_back(0);

	while( k.size() < key.size() ){
	    int current_idx = q.back();
	    k.push_back(current_idx);
	    q.pop_back();
	    int l = left[current_idx];
	    int r = right[current_idx];
	    if( r != -1 ){
		q.push_back(r);
	    }
	    if( l != -1 ){
		q.push_back(l);
	    }
	}
	for(auto & i : k ){
	    i = key[i];
	}
	return k;
    }

    vector <int> post_order() {
        vector<int> k;
        vector<int> q;
	
	q.push_back(0);

	while( k.size() < key.size() ){
	    int current_idx = q.back();
	    k.push_back(current_idx);
	    q.pop_back();
	    int l = left[current_idx];
	    int r = right[current_idx];
	    if( l != -1 ){
		q.push_back(l);
	    }
	    if( r != -1 ){
		q.push_back(r);
	    }
	}
	std::reverse(k.begin(),k.end());
	for(auto & i : k ){
	    i = key[i];
	}
	return k;
    }
};

void print(vector <int> a) {
    for (size_t i = 0; i < a.size(); i++) {
	if (i > 0) {
	    cout << ' ';
	}
	cout << a[i];
    }
    cout << '\n';
}

int main_with_large_stack_space() {
    ios_base::sync_with_stdio(0);
    TreeOrders t;
    t.read();
    print(t.in_order());
    print(t.pre_order());
    print(t.post_order());
    return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
    // Allow larger stack space
    const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
	if (rl.rlim_cur < kStackSize)
	{
	    rl.rlim_cur = kStackSize;
	    result = setrlimit(RLIMIT_STACK, &rl);
	    if (result != 0)
	    {
		std::cerr << "setrlimit returned result = " << result << std::endl;
	    }
	}
    }
#endif

    return main_with_large_stack_space();
}

