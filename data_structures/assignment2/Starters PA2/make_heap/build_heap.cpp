#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
private:
    vector<int> data_;
    vector< pair<int, int> > swaps_;

    void WriteResponse() const {
	cout << swaps_.size() << "\n";
	for (int i = 0; i < swaps_.size(); ++i) {
	    cout << swaps_[i].first << " " << swaps_[i].second << "\n";
	}
    }

    void ReadData() {
	int n;
	cin >> n;
	data_.resize(n);
	for(int i = 0; i < n; ++i)
	    cin >> data_[i];
    }

    void GenerateSwaps() {
	swaps_.clear();
	// for (int i = 0; i < data_.size(); ++i)
	//   for (int j = i + 1; j < data_.size(); ++j) {
	//     if (data_[i] > data_[j]) {
	//       swap(data_[i], data_[j]);
	//       swaps_.push_back(make_pair(i, j));
	//     }
	//   }
	make_heap();
    }
    void make_heap(){
	heapsize = data_.size();
	for( size_t i = data_.size(); i-- != 0; ){
	    sift_down( i );
	}
    }
    
    void sift_down( int index ){
	if( index >= heapsize/2 || index < 0 )
	    return;
	int vmin = data_[index];
	int ileft = index * 2 + 1;
	int iright = index * 2 + 2;
	int vleft = std::numeric_limits<int>::max();
	int vright = std::numeric_limits<int>::max();
	int imin = index;
	if( ileft < heapsize ){
	    vleft = data_[ileft];
	    if( vleft < vmin ){
		imin = ileft;
		vmin = vleft;
	    }
	}
	if( iright < heapsize ){
	    vright = data_[iright];
	    if( vright < vmin ){
		imin = iright;
		vmin = vright;
	    }
	}
	if( imin != index ){
	    int temp = data_[index];
	    data_[index] = vmin;
	    data_[imin] = temp;
	    swaps_.push_back(make_pair(index,imin));
	    //recurse
	    sift_down( imin );
	}
    }
    size_t heapsize;
public:
    void Solve() {
	ReadData();
	GenerateSwaps();
	WriteResponse();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    HeapBuilder heap_builder;
    heap_builder.Solve();
    return 0;
}
