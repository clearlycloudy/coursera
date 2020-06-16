#include <ios>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct ConvertILPToSat {
    vector< vector<int> > A;
    vector<int> b;
    
    ConvertILPToSat(int n, int m) : A(n, vector<int>(m)), b(n)
    {}

    int getid( int row, int col ){
	//return row * b.size() + col;
	return col;
    }
    void printEquisatisfiableSatFormula() {
	vector< vector<int> > clauses {};
	int num_vars = 0;
	map<int,int> map_var_id;
	for( size_t r = 0; r < A.size(); ++r ){ //each row
	    vector<int> arr {};
	    for( size_t c = 0; c < A[r].size(); ++c ){ //pick up col indices where the value is non-zero
		int e = A[r][c];
		if( e != 0 ){
		    arr.push_back(c);
		}
	    }
	    while( arr.size() != 3 ){ //fill empty places
		arr.push_back(-1);
	    }
	    bool num_satisfiable = 0;
	    // if(r==1){
	    // 	cout << "r:1" << endl;
	    // }
	    for( unsigned char i = 0; i < 8; ++i ){ //go through 2^3 combinations of 3 boolean variable assignments
		//dot product of 1 with the associated coefficients or 0 if the index is -1
		int sum = 0;
		for( unsigned char j = 0; j < 3; ++j ){
		    if( ( i & ( 0x01 << j ) ) && arr[j] != -1 ){
			sum += 1 * A[r][ arr[j] ];
		    }
		}
		if( sum > b[r] ){ //unsatisfiable assignment of the 3 boolean variables detected
		    //emit clause that states this unsatisfiable condition
		    // cout << "unsat: " << (int)i << ": " << sum << ", constraint: " << b[r] << endl;
		    // for( auto y : arr ){
		    // 	cout << y << " ";
		    // }
		    // cout << endl;
		    // cout << "---" << endl;
		    vector<int> terms {};
		    for( unsigned char j = 0; j < 3; ++j ){
			// if( b[r] >= 0 && ( i & ( 0x01 << j ) ) && arr[j] != -1 ){
			if( arr[j] != -1 ){
			    if( ( A[r][arr[j]] > 0 ) && ( i & ( 0x01 << j ) ) ){ //negative coefficient and boolean bit is 1
				//cout << A[r][arr[j]] << endl;

				//introduce new variable for SAT solver if necessary
				int id = getid( r, arr[j] );
				auto it = map_var_id.find( id );
				int var_id;
				if( map_var_id.end() == it ){
				    var_id = num_vars;
				    // cout << "added var: " << var_id << endl;
				    map_var_id[ id ] = num_vars++;
				}else{
				    var_id = it->second;
				}
				terms.push_back( -(var_id+1) );
			    }
			    else if( ( A[r][arr[j]] < 0 ) && ( 0 == ( i & ( 0x01 << j ) ) ) ){ //positive coefficient and boolean bit is 0
				//cout << A[r][arr[j]] << endl;

				//introduce new variable for SAT solver if necessary
				int id = getid( r, arr[j] );
				auto it = map_var_id.find( id );
				int var_id;
				if( map_var_id.end() == it ){
				    var_id = num_vars;
				    // cout << "added var: " << var_id << endl;
				    map_var_id[ id ] = num_vars++;
				}else{
				    var_id = it->second;
				}
				terms.push_back( (var_id+1) );
			    }
			}
		    }
		    if( terms.size() > 0 ){
			clauses.push_back( terms );
		    }
		}else{
		    ++num_satisfiable;
		}
	    }
	    if( num_satisfiable == 0 ){ //can return early if no solution for current constraint
		clauses.push_back( { num_vars++ + 1 } );
		clauses.push_back( { -num_vars++ + 1 } );
		break;
	    }
	}

	if( clauses.size() == 0 ){
	    // cout <<"clauses empty" << endl;
	    cout << "1 1" << endl;
	    cout << "1 -1 0" << endl;
	}else{
	    cout << clauses.size() << " " << num_vars << endl;
	    for( auto & i : clauses ){
		for( auto j : i ){
		
		    cout << j << " ";
		}
		cout << 0 << endl;
	    }
	}
    }
};

int main() {
    // ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertILPToSat converter(n, m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        cin >> converter.A[i][j];
      }
    }
    for (int i = 0; i < n; i++) {
      cin >> converter.b[i];
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
