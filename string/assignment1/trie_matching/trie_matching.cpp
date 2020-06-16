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

    Node ()
    {
	fill (next, next + Letters, NA);
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
	if( trie[current_node].isLeaf() ){
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
    {
	for( auto & p : patterns ){
	    int current_node = 0;
	    for( auto & c : p ){
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
		}
	    }
	}
    }
    //match input with trie
    vector <int> result;
    for( int j = 0; j < text.size(); ++j ){
	int k = j;
	int index = match( text, k, trie );
	if( index != -1 )
	    result.push_back(index);
	// int current_node = 0;
	// while(1){
	//     int char_index = letterToIndex(text[k]);
	//     if( trie[current_node].next[char_index] != NA ){
	// 	//found a viable path
	// 	current_node = trie[current_node].next[char_index];
	// 	// cout << j << " found path, k: " << k << endl;
	//     }else{
	// 	//no match
	// 	// cout << j << " no match, k: " << k << endl;
	// 	break;
	//     }
	//     if( trie[current_node].isLeaf() ){
	// 	//found
	// 	result.push_back( j );
	// 	// cout << j << " found match, k: " << k << endl;
	// 	break;
	//     }
	//     ++k;
	//     if( k >= text.size() ){
	// 	break;
	//     }
	// }
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
        if (i + 1 < (int) ans.size ())
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
    }

    return 0;
}
