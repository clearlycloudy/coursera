#include <iostream>
#include <vector>

using namespace std;

int get_change(int m) {
    int count = 0;
    vector<int> denomination {10,5,1};
    for( auto & i : denomination ){
	int quot = m / i;
	int rem = m % i;
	count += quot;
	m -= quot * i;
	if( rem == 0 )
	    break;
    }
    return count;
}

int main() {
    int m;
    std::cin >> m;
    std::cout << get_change(m) << '\n';
}
