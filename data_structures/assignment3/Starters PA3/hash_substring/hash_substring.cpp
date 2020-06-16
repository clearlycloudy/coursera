#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <random>

using namespace std;

typedef long long signed ull;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void print_occurrences(const std::list<int>& output) {
    for( auto & i : output )
        std::cout << i << " ";
    std::cout << "\n";
}

bool are_equal( string const & text, string const & pattern, int index_text ){
    int s = pattern.size();
    auto it = text.begin() + index_text;
    auto it_pat = pattern.begin();
    while( it_pat != pattern.end() ){
	if( *it != *it_pat ){
	    return false;
	}
	++it_pat;
	++it;
    }
    return true;
}

ull hash_func(const string & s, int start, int len ){
    ull const multiplier = 256;
    ull const prime = 100000007; //(2^63)-1
    ull hash = 0;
    for (int i = (int)start + (int)len - 1; i >= (int)start; --i ){
	ull temp = (hash * multiplier) % prime;
	ull temp2 = ( (ull)s[i] ) % prime;
	hash = ( temp + temp2 ) % prime;
    }
    return hash;
}

ull hash_func_2(const string & s, int start, int len ){
    ull const multiplier = 246;
    ull const prime = 2147483647; //(2^63)-1
    ull hash = 0;
    for (int i = (int)start + (int)len - 1; i >= (int)start; --i ){
	ull temp = ((hash * multiplier) % prime + prime ) % prime;
	ull temp2 = ( ( (ull)s[i] ) % prime + prime ) % prime;
	hash = ( ( temp + temp2 ) % prime + prime ) % prime;
    }
    return hash;
}

std::list<int> get_occurrences(const Data& input) {

    list<int> ans {};
    ull precompute_last;
    ull precompute_last2;

    static ull const multiplier = 256;
    static ull const prime = 100000007;

    // static ull const multiplier2 = 246;
    // static ull const prime2 = 2147483647;

    //hash the pattern
    ull pattern_hash = hash_func(input.pattern,0,input.pattern.size());
    // ull pattern_hash2 = hash_func_2(input.pattern,0,input.pattern.size());

    // cout << "pattern_hash: " << pattern_hash << endl;
    //precompute the text
    ull text_hash = hash_func(input.text, input.text.size() - input.pattern.size(),input.pattern.size());
    // ull text_hash2 = hash_func_2(input.text, input.text.size() - input.pattern.size(),input.pattern.size());
    precompute_last = text_hash;
    // precompute_last2 = text_hash2;
    ull polynomial_largest = 1;
    for( int i = 0; i < input.pattern.size(); ++i ){
	polynomial_largest = ( polynomial_largest * multiplier ) % prime;
    }
    // ull polynomial_largest_2 = 1;
    // for( int i = 0; i < input.pattern.size(); ++i ){
    // 	polynomial_largest_2 = ( polynomial_largest_2 * multiplier2 ) % prime2;
    // }
    
    if( precompute_last == pattern_hash ){
	if( are_equal( input.text, input.pattern, input.text.size() - input.pattern.size() ) ){
	    ans.push_front(input.text.size() - input.pattern.size());
	}
    }
    for( int i = input.text.size() - input.pattern.size() - 1; i >= 0; --i ){
	ull temp = (precompute_last * multiplier ) % prime;
	ull temp2 = (polynomial_largest * (ull)input.text[ i + input.pattern.size() ]) % prime;
        precompute_last = ( ( ( temp - temp2 ) % prime + prime ) % prime + (ull)input.text[i] ) % prime;
	// ull temp2_1 = ( precompute_last2 * multiplier2  );
	// ull temp2_2 = ( polynomial_largest_2 * (ull)input.text[ i + input.pattern.size() ]);
	// precompute_last2 = ( (ull)input.text[i] + temp2_1 - temp2_2 ) % prime2;
	//find equal hashes and check actual value
	if( precompute_last == pattern_hash ){
	    if( are_equal( input.text, input.pattern, i ) ){
		ans.push_front(i);
	    }
	}
    }
    return ans;
}


std::list<int> get_occurrences_slow(const Data& input) {
    std::list<int> ans {};
    for( size_t i = 0; i + input.pattern.size() <= input.text.size(); ++i ){
	if( input.text.substr( i, input.pattern.size() ) == input.pattern )
	    ans.push_back(i);
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(true);
    print_occurrences(get_occurrences(read_input()));

    // //stress test
    // while(1){
    // 	Data data;
    // 	data.pattern = "";
    // 	data.text = "";
    // 	// std::default_random_engine gen;
    // 	std::random_device                  rand_dev;
    // 	std::mt19937                        gen(rand_dev());
    // 	std::uniform_int_distribution<int> distr(1,500000);
    // 	int len_text = distr( gen );
    // 	std::uniform_int_distribution<int> distr_pattern(0,len_text-1);
    // 	int pattern_start = distr_pattern( gen );
    // 	std::uniform_int_distribution<int> distr_pattern_end(pattern_start,len_text-1);
    // 	int pattern_end = distr_pattern_end( gen );
    // 	cout << "len_text: " << len_text << endl;
    // 	cout << "pattern_start: " << pattern_start << endl;
    // 	cout << "pattern_end: " << pattern_end << endl;
    // 	std::uniform_int_distribution<char> distr_char(65,122);
    // 	data.text.resize(len_text);
    // 	for( int i = 0; i < len_text; ++i ){
    // 	    char c = distr_char( gen );
    // 	    data.text[i] = c;
    // 	}
    // 	data.pattern = data.text.substr( pattern_start, pattern_end - pattern_start + 1 );

    // 	// cout << "text: " << data.text << ", pattern: " << data.pattern << endl;

    // 	// cin>>data.text >> data.pattern;
	
    // 	std::list<int> fast = get_occurrences(data);
    // 	std::list<int> slow = get_occurrences_slow(data);
    // 	if( fast != slow ){
    // 	    cout << "not match: " << endl;
    // 	    cout << "fast :";
    // 	    for( auto & i : fast ){
    // 		cout << i << " ";
    // 	    }
    // 	    cout <<endl;
    // 	    cout << "slow :";
    // 	    for( auto & i : slow ){
    // 		cout << i << " ";
    // 	    }
    // 	    cout <<endl;
    // 	    getchar();
    // 	}else{
    // 	    cout << "match" << endl;
    // 	}
    // }
    
    return 0;
}
