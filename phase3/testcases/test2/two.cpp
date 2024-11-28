class Solution {
public:

struct Edge {
        public:
        int p1, p2, length;
        Edge(int len, int a, int b) : p1(a), p2(b), length(len) {}
        bool operator> (const Edge &e) const {
            return length > e.length;
        }
    };

    vector<int> parent;

    int root(int node){
        // while(parent[node]!=node) node = parent[node];
        if(parent[node]==node) return node;
        return root(parent[node]);
    }

    bool dsu(Edge e){
        if(root(e.p1)==root(e.p2)) return false;
        parent[root(e.p2)] = root(e.p1);
        return true;
    }

void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    bool firstRowZero = false, firstColZero = false;

    // Check if the first row and first column need to be zeroed
    
    for (int i = 0; i < m; ++i) {
        if (matrix[i][0] == 0) {
            firstColZero = true;
            break;
        }
    }

    for (int j = 0; j < n; ++j) {
        if (matrix[0][j] == 0) {
            firstRowZero = true;
            break;
        }
    }

    // Use first row and column as markers
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            if (matrix[i][j] == 0) {
                matrix[0][j] = 0;
                matrix[i][0] = 0;
            }
        }
    }

    // Zero out cells based on markers
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            if (!matrix[i][0] || !matrix[0][j]) {
                matrix[i][j] = 0;
            }
        }
    }

    // Zero out the first row and column if needed
    if (firstColZero) {
        for (int i = 0; i < m; ++i) {
            matrix[i][0] = 0;
        }
    }
    if (firstRowZero) {
        for (int j = 0; j < n; ++j) {
            matrix[0][j] = 0;
        }
    }
}
};