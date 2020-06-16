#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <sys/resource.h>

using namespace std;

struct Vertex {
    long int weight = 0;
    long int sum_included = 0;
    long int sum_excluded = 0;
    Vertex(){
	weight = 0;
	sum_included = 0;
	sum_excluded = 0;
    }
    std::vector <int> children;
};
typedef std::vector<Vertex> Graph;
typedef std::vector<int> Sum;

Graph ReadTree() {
    int vertices_count;
    std::cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        std::cin >> tree[i].weight;

    for (int i = 1; i < vertices_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);
        tree[to - 1].children.push_back(from - 1);
    }

    return tree;
}

void dfs(Graph &tree, int vertex, int parent ) {
    for (int child : tree[vertex].children){
        if (child != parent){
            dfs(tree, child, vertex);
	}
    }
    //evaluate to trickle sums upward  using dynamic programming
    if( parent != -1 ){
	tree[parent].sum_included += tree[vertex].sum_excluded; //for the case that parent includes its own weight
	tree[parent].sum_excluded += std::max( tree[vertex].sum_included + tree[vertex].weight, tree[vertex].sum_excluded ); //for the case that parent exludes its own weight, either take summed values from subtree including current node's weight or not which ever is max
    }
}

long int MaxWeightIndependentTreeSubset( Graph &tree) {    
    size_t size = tree.size();
    if (size == 0)
        return 0;
    dfs(tree, 0, -1);
    return std::max( tree[0].sum_included + tree[0].weight, tree[0].sum_excluded );
}

int main() {
    // This code is here to increase the stack size to avoid stack overflow
    // in depth-first search.
    const rlim_t kStackSize = 64L * 1024L * 1024L;  // min stack size = 64 Mb
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    // Here begins the solution
    Graph tree = ReadTree();
    int long weight = MaxWeightIndependentTreeSubset(tree);
    std::cout << weight << std::endl;
    return 0;
}
