class Solution {
public:

    struct uf{
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

        void makenode(int node){
            parent[node] = node; size[node] = 1;
        }

        void dsu(Edge e){
            if(size[e.p2]>size[e.p1]) swap(e.p2,e.p1);
            parent[root(e.p2)] = root(e.p1);
            size[root(e.p1)] += size[root(e.p2)];
        }

        int root(int node){
            // while(parent[node]!=node) node = parent[node];
            if(parent[node]!=node){
                parent[node] = root(parent[node]);
            }
            return (parent[node]);
        }

        uf(int len){
            size.resize(len);
            parent.resize(len);
            for(int i=0; i<len; i++) makenode(i);
        }

    };
 

    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        uf gr(n);

        priority_queue<uf::Edge, vector<uf::Edge>, greater<uf::Edge>> pq;
        for(int i=0; i<n; i++){
            for(int j=i+1; j<n; j++){
                int l = abs(points[i][0]-points[j][0]) + abs(points[i][1]-points[j][1]);
                uf::Edge e(l, i, j);
                pq.push(e);
            }
        }

        int done = 0;
        int cost = 0;
        while(!pq.empty()){
            uf::Edge e = pq.top();
            pq.pop();
            cout<<gr.root(e.p1);
            if(gr.root(e.p1)!=gr.root(e.p2)){
                gr.dsu(e);
                cost += e.length;
                done++;
                if(done==n-1) break;
            }
        }
        return cost;
    }
};