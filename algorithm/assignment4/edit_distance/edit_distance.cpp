#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int edit_distance(const string & str1, const string & str2) {
    size_t a = str1.size();
    size_t b = str2.size();

    vector<vector<int>> mem(a, vector<int>(b,0));
    for( int i = 0; i < a; ++i ){
	mem[i][0] = i;
    }
    for( int j = 0; j < b; ++j ){
	mem[0][j] = j;
    }

    for( int i = 1; i < a; ++i ){
	for( int j = 1; j < b; ++j ){
	    //compute all possibilities for current indices
 	    int up = mem[i-1][j];
	    int left = mem[i][j-1];
	    int diag = mem[i-1][j-1];
	    if( str1[i] == str2[j] ){
		vector<int> arr{ up+1, left+1, diag };
		mem[i][j] = *std::min_element(arr.begin(), arr.end());
	    }else{
		vector<int> arr{ up+1, left+1, diag+1 };
		mem[i][j] = *std::min_element(arr.begin(), arr.end());
	    }
	}
    }
    
    return mem[a-1][b-1];
}

int main() {
    string str1;
    string str2;
    std::cin >> str1 >> str2;
    std::cout << edit_distance(" "+str1, " "+str2) << std::endl;
    return 0;
}
