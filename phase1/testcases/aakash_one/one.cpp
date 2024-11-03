#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

bool dfs(int node, int parent, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            if (dfs(neighbor, node, graph, visited)) {
                return true;  // Cycle detected in the recursive call
            }
        } else if (neighbor != parent) {
            return true;  // Cycle detected due to back edge
        }
    }
    return false;
}

pair<int, bool> connectedComponentsAndCycleDetection(unordered_map<int, vector<int>>& graph) {
    unordered_set<int> visited;
    int componentCount = 0;
    bool hasCycle = false;

    for (const auto& [node, _] : graph) {
        if (visited.find(node) == visited.end()) {
            componentCount++;
            if (dfs(node, -1, graph, visited)) {
                hasCycle = true;
            }
        }
    }

    return {componentCount, hasCycle};
}

int main() {
    unordered_map<int, vector<int>> graph;
    graph[0] = {1, 2};
    graph[1] = {0, 2};
    graph[2] = {0, 1, 3};
    graph[3] = {2, 4};
    graph[4] = {3};

    auto [components, cycleExists] = connectedComponentsAndCycleDetection(graph);
    cout << "Total number of connected components: " << components << endl;
    cout << "Cycle exists in the graph: " << (cycleExists ? "Yes" : "No") << endl;

    return 0;
}
