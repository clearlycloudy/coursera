#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <deque>

using namespace std;

class MaxMatching {
public:
    void Solve() {
	int source, sink, num_flights;
	auto adj_matrix = ReadData( source, sink, num_flights );
	auto matching = FindMatching(adj_matrix, source, sink, num_flights);
	WriteResponse(matching);
    }

private:
    map<pair<int,int>, pair<bool,bool> > ReadData( int & source, int & sink, int & num_flights ) {
	int num_left, num_right;
	cin >> num_left >> num_right;
        map<pair<int,int>, pair<bool,bool> > adj_matrix;
	source = num_left + num_right;
        sink = num_left + num_right + 1;

	for (int i = 0; i < num_left; ++i){
	    for (int j = 0; j < num_right; ++j) {
		int bit;
		cin >> bit;
		if( bit == 1 ){
		    adj_matrix[ { i, j+num_left } ] = { false, (bit == 1) };
		    adj_matrix[ { j+num_left, i } ] = { true, (bit == 1) };
		}
	    }
	}
	//source connecting to flights
	for (int i = 0; i < num_left; ++i){
	    adj_matrix[ { num_left + num_right, i } ] = { false, true };
	}
	//sink connecting to crews
	for (int i = 0; i < num_right; ++i){
	    adj_matrix[ { num_left + i, num_left + num_right + 1 } ] = { false, true };
	}

	// cout <<"capacity_matrix " << endl;
	// for( auto & i : adj_matrix ){
	//     cout << i.first.first << ", " << i.first.second <<": " << i.second.second << endl;
	// }
	    
	num_flights = num_left;
	return adj_matrix;
    }

    void WriteResponse(const vector<int>& matching) {
	for (int i = 0; i < matching.size(); ++i) {
	    if (i > 0)
		cout << " ";
	    if (matching[i] == -1)
		cout << "-1";
	    else
		cout << matching[i];
	}
	cout << "\n";
    }
    
    vector<int> FindMatching(map<pair<int,int>, pair<bool,bool> > & adj_matrix, int source, int sink, int num_flights ) {
	int flow = 0;

	while(true){
	    deque<int> q;
	    q.push_back( source ); //source
	    map<int, map< pair<int,int>, pair<bool,bool> >::iterator > path {};
	    
	    while( !q.empty() ){
		int current = q.front();
		q.pop_front();
		//find adjacent edge to current node
		auto it_edge_start = adj_matrix.lower_bound( {current, 0} );
		auto it_edge_end = adj_matrix.lower_bound( {current+1, 0} );
		while(it_edge_start != it_edge_end ){
		    auto & e = it_edge_start->first;
		    auto & flow_info = it_edge_start->second;
		    if( e.first == current ){
			if( path.find( e.second ) == path.end()
			    && it_edge_start->second.second == true
			    && it_edge_start->second.first == false //capacity is not maxed out
			    && e.second != source
			    && e.second != e.first ){
			    // std::cout << "e: " << e.first << "->" << e.second << ", flow: " << flow_info.first << ", cap: " << flow_info.second << std::endl;
			    path[e.second] = it_edge_start; //save the "to" node number to path
			    q.push_back(e.second);
			}
		    }
		    ++it_edge_start;
		}
		// cout << "iteration done" << endl;
	    }
	    if( path.find( sink ) == path.end() ){
		// cout << "break" << endl;
	    	break;
	    } else {
	    	//found a augmenting path
	    	map< pair<int,int>, pair<bool,bool> >::iterator e = path[sink];
	    	auto it = path.find( e->first.second );
	    	//find the edge having smallest additional flow
	    	// while( it != path.end() ){
	    	//     it = path.find( it->second->first.first );
	    	// }
	    	//d is now the maximum amount that can be propagated in a path
	    	//update the flow in that path
	    	// it = path.find( e->first.second );
	    	while( it != path.end() ){
	    	    auto edge_forward = adj_matrix.find( {it->second->first.first, it->second->first.second} );
	    	    auto edge_back = adj_matrix.find( {it->second->first.second, it->second->first.first} );
		    edge_forward->second.first = true;
	    	    edge_back->second.first = false;
	    	    // std::cout << "post_adjust: " << it->second->first.first << " - " << it->second->first.second << endl;
	    	    it = path.find( it->second->first.first );
	    	}
	    	// std::cout << "flow: " << flow << std::endl;
	    }
	    // cout << "iteration end" << endl;

	    // cout <<"adj_matrix " << endl;
	    // for( auto & i : adj_matrix ){
	    // 	cout << i.first.first << ", " << i.first.second <<": " << i.second.first << endl;
	    // }
	}

	// cout <<"adj_matrix " << endl;
	// for( auto & i : adj_matrix ){
	//     cout << i.first.first << ", " << i.first.second <<": " << i.second.first << endl;
	// }
	//gather answer
	vector<int> ans( num_flights, -1 ); //init to -1
	// auto it_edge_start = adj_matrix.lower_bound( {num_flights-1, 0} );
	// auto it_edge_end = adj_matrix.lower_bound( {num_flights-1, 999999} );
	// while( it_edge_start != it_edge_end ){
	for( auto & it_edge_start : adj_matrix ){
	    // cout << "here" << endl;
	    int from = it_edge_start.first.first; //source
	    int to = it_edge_start.first.second; //flight
	    
	    if( it_edge_start.second.first == true && from == source && from != to ){
		for( auto & it_edge_crew : adj_matrix ){
		    if( it_edge_crew.first.first == to && it_edge_crew.second.first == true  &&
			it_edge_crew.first.first != it_edge_crew.first.second ){
			auto crew = it_edge_crew.first.second;
			// cout << from << ":-> " << to << " -> " << crew << endl;
			ans[to] = crew - num_flights + 1; //map flight to crew (1-indexed)
		    }
		}
	    }
	}
        return ans;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    MaxMatching max_matching;
    max_matching.Solve();
    return 0;
}
