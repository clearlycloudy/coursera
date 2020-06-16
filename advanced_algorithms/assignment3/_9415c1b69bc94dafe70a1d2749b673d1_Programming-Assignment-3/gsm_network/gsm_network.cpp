#include <ios>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int from;
    int to;
};

struct ConvertGSMNetworkProblemToSat {
    int numVertices;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {  }

    void EmitSelectOne(vector<int> choices ){
	for( auto i : choices ){
	    cout << i << " ";
	}
	cout << 0 << endl;

	for( size_t i = 0; i < choices.size(); ++i ){
	    for( size_t j = 0; j < choices.size(); ++j ){
		if( i == j ) continue;
		else {
		    cout << -choices[j] << " ";
		}
	    }
	    cout << 0 << endl;
	}
    }
    void EmitXor( int a, int b ){
	cout << a << " " << b << " 0" << endl;
	cout << -a << " " << -b << " 0" << endl;
    }

    void EmitNand( int a, int b ){
	cout << -a << " " << -b << " 0" << endl;
    }

    void printEquisatisfiableSatFormula() {
	//satisfiability criteria:
	//1. only select only one colour for each cell
	//2. neighbouring cells do not have same colour: use xor

	// cout << "cell constraints:" << endl;

	int num_clauses =  4 * numVertices + 3 * edges.size();

	cout << num_clauses << " " << numVertices*3 << endl;
	
	for( int i = 0; i < numVertices; ++i ){
	    vector<int> current_cell;
	    for( int j = 0; j < 3; ++j ){
		current_cell.push_back( i + j * numVertices + 1 );
	    }
	    EmitSelectOne( current_cell );
	}

	// cout << "edge constraints:" << endl;

	for( auto & e : edges ){
	    int f = e.from;
	    int t = e.to;
	    for( size_t i = 0; i < 3; ++i ){
		int f_cell_colour = i * numVertices + f;
		int t_cell_colour = i * numVertices + t;
	        EmitNand( f_cell_colour, t_cell_colour );
	    }
	}
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
