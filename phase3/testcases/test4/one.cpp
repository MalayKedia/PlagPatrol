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
    vector<int> size;

    int root(int node){
        // while(parent[node]!=node) node = parent[node];
        if(parent[node]!=node){
            parent[node] = root(parent[node]);
        }
        return (parent[node]);
    }

    bool dsu(Edge e){
        if(root(e.p1)==root(e.p2)) return false;
        if(size[e.p2]>size[e.p1]) swap(e.p2,e.p1);
        parent[root(e.p2)] = root(e.p1);
        size[root(e.p1)] += size[root(e.p2)];
        return true;
    }

    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        parent.resize(n);
        size.resize(n);

        for(int i=0; i<n; i++) {parent[i] = i; size[i] = 1;}

        priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                int l = abs(points[i][0]-points[j][0]) + abs(points[i][1]-points[j][1]);
                Edge e(l, i, j);
                pq.push(e);
            }
        }

        int done = 0;
        int cost = 0;
        while(!pq.empty()){
            Edge e = pq.top();
            pq.pop();
            if(dsu(e)){
                cost += e.length;
                cout<<e.length<<endl;
                done++;
                if(done==n-1) break;
            }
        }
        return cost;
    }
};