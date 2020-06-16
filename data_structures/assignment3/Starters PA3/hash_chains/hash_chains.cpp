#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct Query {
    string type, s;
    size_t ind;
};

class QueryProcessor {
    int bucket_count;
    //hash table with chaining
    vector<list<string>> elems;
    size_t hash_func(const string& s) const {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;
        return hash % bucket_count;
    }

public:
    explicit QueryProcessor(int bucket_count): bucket_count(bucket_count), elems(bucket_count,list<string>{} ) {}

    Query readQuery() const {
        Query query;
        cin >> query.type;
        if (query.type != "check")
            cin >> query.s;
        else
            cin >> query.ind;
        return query;
    }

    void writeSearchResult(bool was_found) const {
        std::cout << (was_found ? "yes\n" : "no\n");
    }

    void processQuery(const Query& query) {
        if (query.type == "check") {
            // use reverse order, because we append strings to the end
	    for( auto i : elems[query.ind] ){	
		std::cout << i << " ";
	    }
            std::cout << "\n";
        } else {
	    size_t hash_query = hash_func( query.s );
            if (query.type == "find"){
		for( auto & i : elems[hash_query] ){
		    if( i == query.s ){
			writeSearchResult(true);
			return;
		    }
		}
		writeSearchResult(false);
            }else if (query.type == "add") {
		bool exists = false;
		for( auto & i : elems[hash_query] ){
		    if( i == query.s ){
			exists = true;
			break;
		    }
		}
		if( !exists ){
		    elems[hash_query].push_front(query.s);
		}
            }else if (query.type == "del") {
		for( auto it = elems[hash_query].begin(), it_end = elems[hash_query].end(); it != it_end; ++it ){
		    if( *it == query.s ){
			elems[hash_query].erase(it);
			break;
		    }
		}
            }
        }
    }

    void processQueries() {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            processQuery(readQuery());
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    int bucket_count;
    cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.processQueries();
    return 0;
}
