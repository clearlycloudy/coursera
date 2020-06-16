#include <bits/stdc++.h>
#include <set>
#include <utility>
using namespace std;

struct Edge {
    int from;
    int to;
};

struct ConvertHampathToSat {
    int numVertices;
    vector<Edge> edges;
    std::set< std::pair<int,int> > edges_preprocess;
    // map< pair<int,int> > m_id;
    int id;
    
    ConvertHampathToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {
	id = 0;
    }

    int getid( int path, int vert ){
    	return vert + path * numVertices + 1;
    }

    void printEquisatisfiableSatFormula() {

	vector< vector<int> > clauses {};

	//preprocess the corridors
	for( auto i : edges ){
	    int a = i.from;
	    int b = i.to;
	    if( i.from > i.to ){
		std::swap( a, b );
	    }
	    std::pair<int,int> e = { a, b };
	    edges_preprocess.insert( e );
	}

	//each vertex is used in the hamiltonian path
	for( size_t j = 0; j < numVertices; ++j ){ //each vert j
	    vector<int> c;
	    for( size_t i = 0; i < numVertices; ++i ){ //each path i
	        int v = getid( i, j ); //vert j path i
		c.push_back( v );
	    }
	    clauses.push_back( c );
	}

	// cout << "clause 2" << endl;

	//no vertex appears twice in the path
	for( size_t i = 0; i < numVertices; ++i ){ //path i
	    for( size_t k = i+1; k < numVertices; ++k ){ //path k
		for( size_t j = 0; j < numVertices; ++j ){ //each vert j
		    int v1 = getid( i, j ); //vert j path i
		    int v2 = getid( k, j ); //vert j path k
		    vector<int> c;
		    c.push_back( -v1 );
		    c.push_back( -v2 );
		    clauses.push_back( c );
		}
	    }
	}

	// cout << "clause 3" << endl;
	 
	//path slots must be occupied
	for( size_t i = 0; i < numVertices; ++i ){ //each path i
	    vector<int> c;
	    for( size_t j = 0; j < numVertices; ++j ){ //each vert j
		size_t v = getid( i, j ); //vert j path i
		c.push_back( v );
	    }
	    clauses.push_back( c );
	}

	// cout << "clause 4" << endl;
	
	//no 2 vertices occupy the name slot in path
	for( size_t j = 0; j < numVertices; ++j ){ //each vert j
	    for( size_t k = j+1; k < numVertices; ++k ){ //each vert k
		for( size_t i = 0; i < numVertices; ++i ){ //each path i
		    int v1 = getid( i, j ); //vert j path i
		    int v2 = getid( i, k ); //vert k path i
		    vector<int> c;
		    c.push_back( -v1 );
		    c.push_back( -v2 );
		    clauses.push_back( c );
		}
	    }
	}

	size_t index_clause5 = clauses.size();

	//non-adjacent vertices cannot be adjacent in the path
//	    cout << "k: " << k << endl;
	for( size_t i = 0; i < numVertices; ++i ){
	    for( size_t j = 0; j < numVertices; ++j ){
		if( i == j ) continue;
		if( edges_preprocess.end() == edges_preprocess.find( { i + 1, j + 1 } ) && 
		    edges_preprocess.end() == edges_preprocess.find( { j + 1, i + 1 } ) ){
		    for( size_t k = 0; k < numVertices-1; ++k ){ //each path
			int path1 = getid( k, i ); //vert i path k
			int path2 = getid( k+1, j ); //vert j path k+1
			if( path1 < path2 ){
			    // cout << -path1 << " " << -path2 << " ";
			    // cout << 0 << endl;
			    vector<int> c;
			    c.push_back( -path1 );
			    c.push_back( -path2 );
			    clauses.push_back( c );
			}
		    }
		}
	    }
	}
	//number of clauses
	cout << clauses.size() << " " << numVertices * numVertices << endl;

	size_t index = 0;
	for( auto & i : clauses ){
	    // if( index == index_clause5 ){
	    // 	cout << "clause 5" << endl;
	    // }
	    for( auto j : i ){
		cout << j << " ";
	    }
	    cout << 0 << endl;
	    ++index;
	}
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertHampathToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
