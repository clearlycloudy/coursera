#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

struct Segment {
    int start, end;
};

vector<int> optimal_points(vector<Segment> &segments) {
    vector<int> points;

    sort( segments.begin(), segments.end(), []( Segment const & a, Segment const & b )->bool{

	    if( a.end < b.end ) return true;
	    else if( a.end > b.end ) return false;
	    else{
		if( a.start < b.start ) return true;
		else return false;
	    }
	});

    for (size_t i = 0; i < segments.size(); ) {
	int point = segments[i].end;
	points.push_back(point);
	size_t j = i + 1;
	for( ; j < segments.size(); ++j){
	    if( point < segments[j].start ){
		break;
	    }
	}
	i = j;
    }
    return points;
}

int main() {
    int n;
    std::cin >> n;
    vector<Segment> segments(n);
    for (size_t i = 0; i < segments.size(); ++i) {
	std::cin >> segments[i].start >> segments[i].end;
    }
    vector<int> points = optimal_points(segments);
    std::cout << points.size() << "\n";
    for (size_t i = 0; i < points.size(); ++i) {
	std::cout << points[i] << " ";
    }
    std::cout << std::endl;
}
