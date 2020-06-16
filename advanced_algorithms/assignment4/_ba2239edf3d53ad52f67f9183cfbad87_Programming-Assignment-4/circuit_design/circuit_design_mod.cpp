// #include <bits/stdc++.h>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// #define SANITY_CHECK
struct Clause {
    int firstVar;
    int secondVar;
};

struct Node {
    unordered_set<int> adj;
};

void explore( int i, unordered_map<int, set<int>> const & adj, unordered_map<int,int> & components, int componentcount ){
    if( components.find(i) == components.end() ){
	components[i] = componentcount;
	auto it = adj.find( i );
	if( it != adj.end() ){
	    // for( int j = 0; j < it->second.size(); ++j ){
	    for( auto j : it->second ){
	        // const set<int> & v = it->second; 
		// explore( v[j], adj, components, componentcount );
		explore( j, adj, components, componentcount );
	    }
	}
    }
}

void dfs( int current, vector<int> & visit_order, unordered_map<int, Node> const & imp, unordered_set<int> & visited ){
    if( visited.find( current ) != visited.end() ){
	return;
    }
    visited.insert( current );
    // cout << "visit: " << current << endl;
    auto it = imp.find( current );
    if( it != imp.end() ){
	Node const & n = it->second;
	for( auto i : n.adj ){ //visit neighbours
	    // cout << "visit neighbour: " << current << "->" << i << endl;
	    dfs( i, visit_order, imp, visited );
	}
    }
    //save nodes in post traversal order
    visit_order.push_back( current );
}

struct TwoSatisfiability {
    int numVars;
    vector<Clause> clauses;

    TwoSatisfiability(int n, int m) :
        numVars( n ),
        clauses( m )
    {
    }
    
    bool isSatisfiable(vector<bool>& result) {
	//construct implication graph from formula
        unordered_map<int, Node> imp;
	unordered_set<int> implication_start_nodes;
	for( auto const & i : clauses ){
	    int a = i.firstVar;
	    int b = i.secondVar;
	    int neg_a = -a;
	    int neg_b = -b;
	    if( imp.find(neg_a) == imp.end() ){
		imp[neg_a] = Node();
	    }
	    imp[neg_a].adj.insert( b );
	    if( imp.find(neg_b) == imp.end() ){
		imp[neg_b] = Node();
	    }
	    imp[neg_b].adj.insert( a );

	    implication_start_nodes.insert(neg_a);
	    implication_start_nodes.insert(neg_b);
	}

	// for( auto & i : imp ){
	//     cout << "node: " << i.first << "[ ";
	//     for( auto j : i.second.adj ){
	// 	cout << j << " ";
	//     }
	//     cout << " ]" << endl;
	// }

	//compute strongly connected components using Tarjan's algorithm
        vector<int> visit_order {};
        unordered_set<int> visited;
	for( auto i : implication_start_nodes ){
	    int current_node = i;
	    // cout << "dfs: " << current_node << endl;
	    dfs( current_node, visit_order, imp, visited );
	}
	std::reverse( visit_order.begin(), visit_order.end() );
	// cout << "post traversal order: ";
	// for( auto i : visit_order ){
	//     cout << i << " ";
	// }
	// cout << endl;

	unordered_map<int, set<int> > reverse_graph;
	for( auto i : implication_start_nodes ){
	    for( auto j : imp[i].adj ){
		int a = i;
		int b = j;
		//cout << "reverse graph push: " << b << ": " << a << endl;
		if( reverse_graph.find( b ) == reverse_graph.end() ){
		    reverse_graph[b] = set<int>();
		}
		reverse_graph[b].insert( a );
	    }
	}

	// cout << "reverse implication graph:" << endl;
	// for( auto & i : reverse_graph ){
	//     cout << i.first << ": ";
	//     for( auto j : i.second ){
	// 	cout << j << " ";
	//     }
	//     cout << endl;
	// }

	unordered_map<int,int> components; //map node to strongly connected component
	int num_components = 0;
	for( auto i : visit_order ){
	    if( components.find( i ) == components.end() ){
		++num_components;
		explore( i, reverse_graph, components, num_components );
	    }
	}
	// cout << "component map: " << endl;
	// for( auto i : components ){
	//     cout << i.first << ": " << i.second << endl;
	// }

	//check for infeasibility condition: if a strongly connected component contains a veriable and its negation
	for( int i = 1; i <= numVars; ++i ){
	    if( components.find( i ) != components.end() &&
		components.find( -i ) != components.end() ){
		if( components[i] == components[-i] ){
		    return false;
		}
	    }
	}

	//sort strongly connected components by a topological ordering
	unordered_map<int,unordered_set<int> > ssc_to_nodes;
	unordered_map<int,unordered_set<int> > ssc_dependency;
	for( auto i : components ){
	    if( ssc_to_nodes.find( i.second ) == ssc_to_nodes.end() ){
		ssc_to_nodes[ i.second ] = unordered_set<int>();
	    }
	    ssc_to_nodes[ i.second ].insert( i.first );
	    if( ssc_dependency.find( i.second ) == ssc_dependency.end() ){
		ssc_dependency[ i.second ] = unordered_set<int>();
	    }
	    // for( auto j : imp[ i.first ] ){
	    // 	if( components[ j ] != i.second ){
	    // 	    // cout << "outgoing edge between ssc: " << i.first << ": " << j << endl;
	    // 	    ssc_dependency[ i.second ].insert( components[ j ] );
	    // 	}
	    // }
	    
	    for( auto j : reverse_graph[ i.first ] ){
	    	if( components[ j ] != i.second ){
	    	    // cout << "outgoing edge between ssc: " << i.second << ": " << i.first << ": " << j << endl;
	    	    ssc_dependency[ components[ j ] ].insert( i.second );
	    	}
	    }
	}
	// for( auto & i : ssc_to_nodes ){
	//     cout << "ssc to node map: " << i.first << ": ";
	//     for( auto j : i.second ){
	// 	cout << j << " ";
	//     }
	//     cout << endl;
	// }

	// for( auto & i : ssc_dependency ){
	//     cout << "ssc dependency: " << i.first << ": ";
	//     for( auto j : i.second ){
	// 	cout << j << " ";
	//     }
	//     cout << endl;
	// }
	
	vector<int> ssc_topo_sort;
	std::unordered_set<int> ssc_unvisited;
	std::unordered_set<int> ssc_roots;
	for( auto & i : ssc_dependency ){
	    if( i.second.empty() ){
		ssc_roots.insert( i.first );
	    }else{
		ssc_unvisited.insert( i.first );
	    }
	}
	while( !ssc_roots.empty() ){
	    int s = *ssc_roots.begin();
	    ssc_roots.erase(s);
	    ssc_topo_sort.push_back( s );
	    auto it = ssc_unvisited.begin();
	    while( it != ssc_unvisited.end() ){
		int f = *it;
		ssc_dependency[ f ].erase( s );
		if( ssc_dependency[ f ].empty() ){
		    it = ssc_unvisited.erase( it );
		    ssc_topo_sort.push_back( f );
		}else{
		    it++;
		}
	    }
	}

	// cout << "ssc topo sort of forward implication graph: ";
	// for( auto i : ssc_topo_sort ){
	//     cout << i << " ";
	// }
	// cout << endl;

	//assign 1 to variable and 0 to negation of variable within
	//each ssc in ssc_topo_sort order in order to fulfill implication condition
	unordered_set<int> literal_assigned;
	for( auto i : ssc_topo_sort ){
	    for( auto j : ssc_to_nodes[i] ){
		if( literal_assigned.find( abs(j) ) == literal_assigned.end() ){
		    literal_assigned.insert( abs(j) );
		    if( j > 0 ){
			result[j-1] = true;
		    }else{
			result[-j-1] = false;
		    }
		}
	    }
	}
	// cout << "result" << endl;
	// for( auto i : result ){
	//     cout << i << endl;
	// }
#ifdef SANITY_CHECK
	cout << "sanity check" << endl;
	for( auto const & i : clauses ){
	    bool a = result[abs(i.firstVar)-1] ? i.firstVar : -i.firstVar;
	    bool b = result[abs(i.secondVar)-1] ? i.secondVar : -i.secondVar;
	    if( !a && !b ){
		cout << "assignment is not good. " << endl;
		getchar();
		return false;
	    }
	}
#endif	
	return true;
    }
};

int main() {
    //ios::sync_with_stdio(true);

    int n, m;
    cin >> n >> m;
    TwoSatisfiability twoSat(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
    }

    vector<bool> result(n, true);
    if (twoSat.isSatisfiable(result)) {
        cout << "SATISFIABLE" << endl;
        for (int i = 1; i <= n; ++i) {
            if (result[i-1]) {
                cout << i;
            } else {
                cout << -i;
            }
	    cout << " ";
        }
	cout << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
