#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <chrono>
#include <deque>
#include <tuple>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <random>
#include <cstdio>
#include <deque>

using namespace std;

using std::vector;
typedef vector<vector<int> > Matrix;

const int INF = 1e9;

Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed1);

Matrix read_data_rand() {
	
    std::uniform_int_distribution<int> distribution(2, 10);

    int vertex_count = distribution(generator);

    std::uniform_int_distribution<int> distribution2(1,vertex_count*(vertex_count-1)/2);

    int edge_count = distribution2(generator);

    std::uniform_int_distribution<int> distribution3(1,vertex_count);

    std::uniform_int_distribution<int> distribution4(1,20);
	
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));

    for (int i = 0; i < edge_count; ++i) {
	
	int from = distribution3(generator);

	int to = distribution3(generator);

	while( to == from ){
	    to = distribution3(generator);
	}

	int weight = distribution4(generator);
	// cout << "from, to, weight: " << from << ", " << to << ", " << weight << endl;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

//too slow for this particular problem
void dfs_explore( Matrix const & graph, int c, vector<int> & path, int sum, int & best, unordered_set<int> & visiting, vector<int> & best_path ){
    if( visiting.find(c) != visiting.end() ){ //already in path
	return;
    }
    int value = 0;
    if( path.size() > 0 ){
	value = graph[path.back()][c];
    }
    //bounding value
    sum += value;
    if( sum > best ){
	return;
    }

    //admit node
    path.push_back(c);
    visiting.insert(c);
	
    //end condition
    if( path.size() == graph.size() ){
	int close = graph[c][path.front()];
	if( close != INF ){
	    if( sum + close < best ){
		best = sum + close;
		best_path = path;
	    }
	}
	visiting.erase(path.back());
	path.pop_back();
	return;
    }

    //explore neighbours
    for( int j = 0; j < graph.size(); ++j ){
	if( c != j && graph[c][j] != INF ){
	    dfs_explore( graph, j, path, sum, best, visiting, best_path );
	}
    }
    visiting.erase(path.back());
    path.pop_back();
    return;
}

// struct pair_hash {
//     template <class T1, class T2>
//     std::size_t operator () (const std::pair<T1,T2> &p) const {
// 	auto h1 = std::hash<T1>{}(p.first);
// 	auto h2 = std::hash<T2>{}(p.second);

// 	// Mainly for demonstration purposes, i.e. works but is overly simple
// 	// In the real world, use sth. like boost.hash_combine
// 	return h1 ^ h2;
//     }
// };

std::pair<int, vector<int> > solve_dp(Matrix const & graph){
    vector<vector<int> > c((1 << graph.size())+1, vector<int>(graph.size()+1,INF) );
    
    // cout << "graph: " << graph.size() << endl;
    // cout << "table size: " << c.size() << endl;

    unordered_map<int,vector<int> > bitsets;
    for( unsigned l = 1; l < (1 << graph.size()); ++l ){
	int m = l;
	int h = l;
	int bits = 0;
	while( h > 0 ){
	    if( h & 0x01 ){
		bits++;
	    }
	    h = h >> 1;
	}
	if( bitsets.find(bits) == bitsets.end() ){
	    bitsets[bits] = {m};
	}else{
	    bitsets[bits].push_back(m);
	}
    }

    // for( auto & i : bitsets ){
    // 	cout << i.first << ": ";
    // 	for( auto j : i.second ){
    // 	    cout << j << " ";
    // 	}
    // 	cout << endl;
    // }
    
    c[1][1] = 0;
    map< pair<int,int>, int > path;
    map<int, map<int, int > > backtrack;
    for( int s = 2; s <= graph.size(); ++s ){
	// cout << "s:" << s << endl;
	for( unsigned l : bitsets[s] ){ //for each bitset with s number of bits equal to 1
	    // cout << "l: " << l << endl;
	    int m = l;
	    int h = l;
	    int bits = s;
	    // cout << "m: " << m << ", bits: " << bits << endl;
	    c[m][1] = INF;
	    for( int i = 1; i <= graph.size(); ++i ){ //for each bit
		if( i != 1 && (m & (1 << (i-1))) ){ //exclude node 1 and consider only 1 bits
		    for( int j = 1; j <= graph.size(); ++j ){
			if( j != i ){
			    if( graph[j-1][i-1] != INF ){
				// cout << "m: " << m << ", i: " << i << ", j: " << j << endl;
				//c[m][i] = std::min( c[m][i], c[m ^ (1 << (i-1))][j] + graph[j-1][i-1] );
				if( ( c[m ^ (1 << (i-1))][j] + graph[j-1][i-1] ) < c[m][i] ){
				    c[m][i] = c[m ^ (1 << (i-1))][j] + graph[j-1][i-1];
				    path[{j,i}] = 1;
				    if( c[m][i] != INF ){
					backtrack[m][i] = j;
				    }
				}
			    }
			}
		    }	    
		}
	    }
	}
    }

    // cout << "graph" << endl;
    // for( auto & i : graph ){
    // 	for( auto j : i ){
    // 	    if( j == INF ){
    // 		cout << "-- ";
    // 	    }else{
    // 		cout << j << " ";
    // 	    }
    // 	}
    // 	cout << endl;
    // }

    int best = INF;
    int node = -1;
    for( int i = 1; i <= graph.size(); ++i ){
    	if( graph[i-1][0] != INF ){
    	    if( c[(1 << graph.size())-1][i] + graph[i-1][0] < best ){
    		best = c[(1 << graph.size())-1][i] + graph[i-1][0];
    		node = i;
		// cout << "last path val: " << graph[i-1][0] << endl;
		path[{(1 << graph.size())-1,1}] = 1;
		c[(1 << graph.size())][1] = best;
		backtrack[(1 << graph.size())][1] = i;
    	    }
    	}
    }

    // cout << "backtrack: " << endl;;
    // for( auto & i : backtrack ){
    // 	cout << i.first << ": " << endl;
    // 	for( auto & j : i.second ){
    // 	    cout << j.first << ": " << j.second << endl;
    // 	}
    // 	// cout << endl;
    // }

    // cout << "dp table" << endl;
    // int g = 0;
    // for( auto & i : c ){
    // 	cout << "key: " << g++ << " ";
    // 	for( auto j : i ){
    // 	    if( j == INF ){
    // 		cout << "-- ";
    // 	    }else{
    // 		cout << j << " ";
    // 	    }
    // 	}
    // 	cout << endl;
    // }

    // cout << "path: " << endl;
    // for( auto & i : path ){
    // 	cout << i.first.first << ", " << i.first.second << ": " << i.second << endl;
    // }

    if( best == INF )
    {
    	return { -1, {} };
    }

    // cout << "best: " << best << ", node: " << node << endl;

    // cout << "graph: " << endl;
    // for( auto & i : graph ){
    // 	for( auto j : i ){
    // 	    if( j == INF ){
    // 		cout << "-- ";
    // 	    }else{
    // 		cout << j << " ";
    // 	    }
    // 	}
    // 	cout << endl;
    // }
    vector<int> best_path{};
    best_path.push_back(1);
    int state = 1 << graph.size();
    int xor_bit = backtrack[state][1];
    int xor_bit_prev;
    state = (1 << graph.size()) - 1;
    best_path.push_back(xor_bit);

    while( best_path.size() != graph.size() ){
	// cout << "state: " << state << endl;
	// cout << "xor_bit: " << xor_bit << endl;
	int state_prev = state ^ ( 1 << (xor_bit-1) );
	// cout << "state_prev: " << state_prev << endl;
	xor_bit_prev = backtrack[state][xor_bit];
	// cout << "xor_bit_prev: " << xor_bit_prev << endl;
	best_path.push_back(xor_bit_prev);
	state = state_prev;
	xor_bit = xor_bit_prev;
    }

    // cout << "best path: ";
    // for( auto i : best_path ){
    // 	cout << i << " ";
    // }
    // cout << endl;
    return { best, best_path };
}

// #define STRESS_TEST 1

std::pair<int, vector<int> > optimal_path(Matrix graph) {
    // This solution tries all the possible sequences of stops.
    // It is too slow to pass the problem.
    // Implement a more efficient algorithm here.

    auto ret = solve_dp( graph );
    
#ifdef STRESS_TEST
  
    int c = 0;
    vector<int> path_2;
    int sum = 0;
    int best = INF;
    unordered_set<int> visiting;
    vector<int> best_path;
    dfs_explore( graph, 0, path_2, sum, best, visiting, best_path );

    if( best == INF ){
	best = -1;
    }

    for( auto & i : best_path ){
	++i;
    }

    bool fail = false;
    if( best != ret.first ){
	fail = true;
    }else if( ( best == ret.first ) && ( best != -1 ) ){
	auto best_path_rev_it = best_path.begin() ;
	int front = *best_path_rev_it;
	best_path_rev_it++;
        vector<int> best_path_rev(best_path_rev_it, best_path.end());
	best_path_rev.push_back( front );
	std::reverse(best_path_rev.begin(), best_path_rev.end());
	if( best_path != ret.second && best_path_rev != ret.second ){
	    fail = true;
	}
    }
    if( fail ){
	cout << "graph" << endl;
	for( auto & i : graph ){
	    for( auto j : i ){
		if( j == INF ){
		    cout << "-- ";
		}else{
		    cout << j << " ";
		}
	    }
	    cout << endl;
	}
	
	cout << "dfs: " << best << ": ";
	for( auto i : best_path ){
	    cout << i << " ";
	}
	cout << endl;

	cout << "dp: " << ret.first << ": ";
	for( auto i : ret.second ){
	    cout << i << " ";
	}
	cout << endl;
	return { -2, {} };
    }
#endif
    return ret;
}

int print_answer(const std::pair<int, vector<int> >& answer) {
    std::cout << answer.first << "\n";
    if (answer.first == -1)
        return 0;
    if (answer.first == -2)
        return 1;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << "\n";
    return 0;
}

int main() {
#ifdef STRESS_TEST
    long long count_test = 0;
    while(1){
    	count_test++;
    	if( 0 != print_answer(optimal_path(read_data_rand()))){
    	    int g = 8;
    	    cout << "Test: " << count_test << " failed." << endl;
    	    getchar();
    	    // return 0;
    	}else{
    	    cout << "Test: " << count_test << " passed." << endl;
    	    // return 0;
    	}
    }
#else
    print_answer(optimal_path(read_data()));
#endif
    return 0;
}
