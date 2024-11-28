/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
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
    
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        vector<int> temp;
        queue<pair<TreeNode*, int> > q;
        q.push({root, 0});
        // temp.push(root->val);
        int i=q.size();
        for(; i>0; i=q.size()){
            
            int dist = q.front().second;
            TreeNode* curr = q.front().first;
            q.pop();

            
            if(curr) temp.push_back(curr->val);
            if(curr) q.push({curr->left, dist+1});
            if(curr) q.push({curr->right, dist+1});
            
            if(dist!=q.front().second){
                if(!temp.empty())ans.push_back(temp);
                temp.clear();
            }
            
        }
        i=1;
        while(i<ans.size()){
            for(int j=0; j<ans[i].size()/2; j++){
                int temp = ans[i][ans[i].size()-1-j];
                ans[i][ans[i].size()-1-j] = ans[i][j];
                ans[i][j] = temp;
            }
            i++; i++;
        }
        return ans;
    }
};