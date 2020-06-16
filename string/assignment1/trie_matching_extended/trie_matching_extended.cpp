#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int const Letters =    4;
int const NA      =   -1;

class Node
{
public:
    int next [Letters];
    bool is_end;
    Node ()
    {
	fill (next, next + Letters, NA);
	is_end = false;
    }

    bool isLeaf () const
    {
	return (next[0] == NA && next[1] == NA && next[2] == NA && next[3] == NA);
    }
};

int letterToIndex (char letter)
{
    switch (letter)
    {
    case 'A': return 0; break;
    case 'C': return 1; break;
    case 'G': return 2; break;
    case 'T': return 3; break;
    default: assert (false); return -1;
    }
}

int match( string const & text, int j, vector<Node> & trie ){
    int k = j;
    int current_node = 0;
    while(1){
	int char_index = letterToIndex(text[k]);
	if( trie[current_node].next[char_index] != NA ){
	    //found a viable path
	    current_node = trie[current_node].next[char_index];
	}else{
	    //no match
	    return -1;
	}
	if( trie[current_node].isLeaf() || trie[current_node].is_end ){
	    //found
	    return j;
	}
	++k;
	if( k >= text.size() ){
	    //no match
	    return -1;
	}
    }
}

vector <int> solve (const string& text, int n, const vector <string>& patterns)
{
    //build trie
    vector<Node> trie { Node() }; //populate root node
    trie.reserve(1000000);
    {
	for( auto & p : patterns ){
	    int current_node = 0;
	    for( int i = 0; i < p.size(); ++i ){
		char c = p[i];
		int index = letterToIndex( c );
		if( trie[current_node].next[index] == NA ){
		    //create new node
		    trie.push_back( Node() );
		    trie[current_node].next[index] = trie.size()-1;
		    current_node = trie.size()-1;
		    // cout << "new node: " << current_node << endl;
		}else{
		    //existing node
		    current_node = trie[current_node].next[index];
		    //set flag for an internal node if this is the last character for the current pattern
		    if( i == p.size() -1 ){
			trie[current_node].is_end = true;
		    }
		}
	    }
	}
    }
    //match input with trie
    vector <int> result;
    result.reserve(1000000);
    for( int j = 0; j < text.size(); ++j ){
	int k = j;
	int index = match( text, k, trie );
	if( index != -1 )
	    result.push_back(index);
    }
    return result;
}

int main (void)
{
    string t;
    cin >> t;

    int n;
    cin >> n;

    vector <string> patterns (n);
    for (int i = 0; i < n; i++)
    {
        cin >> patterns[i];
    }

    vector <int> ans;
    ans = solve (t, n, patterns);

    for (int i = 0; i < (int) ans.size (); i++)
    {
        cout << ans[i];
        // if (i + 1 < (int) ans.size ())
        // {
        //     cout << " ";
        // }
        // else
        // {
        //     cout << endl;
        // }
	cout << endl;
    }

    return 0;
}
