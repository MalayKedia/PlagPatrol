#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

bool dfs(int node, const vector<vector<int>>& graph, unordered_set<int>& visited, int parent) {
    visited.insert(node);

    for (int neighbor : graph[node]) {
        if (visited.count(neighbor) == 0) {
            if (dfs(neighbor, graph, visited, node)) {
                return true; // Cycle detected
            }
        } else if (neighbor != parent) {
            return true; // Backedge found, cycle detected
        }
    }

    return false;
}

int count_connected_components_and_detect_cycles(const vector<vector<int>>& graph) {
    int count = 0;
    unordered_set<int> visited;
    bool has_cycle = false;

    for (int i = 0; i < graph.size(); ++i) {
        if (visited.count(i) == 0) {
            count++;
            if (dfs(i, graph, visited, -1)) {
                has_cycle = true;
            }
        }
    }

    cout << "Number of connected components: " << count << endl;
    cout << "Cycle detected: " << (has_cycle ? "Yes" : "No") << endl;

    return count;
}

int main() {
    vector<vector<int>> graph = {
        {1, 2},
        {0, 2, 3},
        {0, 1, 3},
        {1, 2, 4},
        {3}
    };

    count_connected_components_and_detect_cycles(graph);

    return 0;
}