#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using std::vector;

struct item {
    double val_per_weight;
    int weights;
    int values;
};

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
  double value = 0.0;

  vector<item> rank(weights.size());

  for( int i = 0; i < weights.size(); ++i ){
      item it;
      it.val_per_weight = (double)values[i] / weights[i];
      it.weights = weights[i];
      it.values = values[i];
      rank[i] = it;
  }
  std::sort( rank.begin(), rank.end(), []( item const & a, item const & b ){ return a.val_per_weight > b.val_per_weight; } );

  for( auto & i : rank ){
      if( capacity == 0 )
	  break;
      int weight_take = std::min( capacity, i.weights );
      capacity -= weight_take;
      value += i.val_per_weight * weight_take;
  }

  return value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values);

  std::cout.precision(10);
  std::cout << optimal_value << std::endl;
  return 0;
}
