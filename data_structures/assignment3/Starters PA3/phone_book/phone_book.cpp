#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

struct Query {
    string type, name;
    int number;
};

struct Record {
    string name;
    bool exists;
    Record(){
	exists = false;
    }
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const list <string>& result) {
    for( auto & i : result )
        std::cout << i << "\n";
}

list<string> process_queries(const vector<Query>& queries) {
    list<string> result;
    // Keep list of all existing (i.e. not deleted yet) contacts.
    vector<Record> contacts( 10000000 );
    for (size_t i = 0; i < queries.size(); ++i)
        if (queries[i].type == "add") {
	    contacts[queries[i].number].name = queries[i].name;
	    contacts[queries[i].number].exists = true;
        } else if (queries[i].type == "del") {
	    contacts[queries[i].number].exists = false;
	    contacts[queries[i].number].name = "";
        } else {
	    if( contacts[queries[i].number].exists == false ){
		result.push_back( "not found" );
	    }else{
		result.push_back( contacts[queries[i].number].name );
	    }
        }
    return result;
}

int main() {
    write_responses(process_queries(read_queries()));
    return 0;
}
