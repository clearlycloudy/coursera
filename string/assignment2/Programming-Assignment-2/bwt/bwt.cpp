#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


string BWT(const string& text) {
  // write your code here
  vector<string> m ( text.size() );
  m[0] = text;
  for( int i = 1; i < text.size(); ++i ){
      m[i] = m[i-1].substr(1, string::npos) + m[i-1].substr(0,1);
  }
  sort( m.begin(), m.end() );

  string result {};
  result.resize(text.size());
  for( int i = 0; i < text.size(); ++i ){
      result[i] = m[i].back();
  }
  return result;
}

int main() {
  string text;
  cin >> text;
  cout << BWT(text) << endl;
  return 0;
}
