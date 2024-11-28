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

bool Bsearch(int x, int A[], int start, int size){

	int half=size/2;
	if(size==1) return (A[start]==x);
	if(x<A[start+half])
		return Bsearch(x, A, start, half);
	else 
		return Bsearch(x, A, start+half, size-half);

}

double subavg(double arr[], int start, int size){

	double sum=0; 
	for(int i=0; i<size; i++){
		sum=sum+arr[i+start];
	}
	return sum/size;

}
void junk(){

	int n; cin>>n;    //n is the number to be checked
	bool cross[n-1];  //each index corresponds to num i+2
	for(int i=0; i<n-1; i++) cross[i]=0;   //uncrossed 0, crossed 1
	
	for(int i=2; i<n; i++){
		for(int j=1; j*i<=n; j++){
			if(!cross[j*i-2]) cross[j*i-2]=1;
		}
	}
	
	if(cross[n-2]) cout<<n<<" is composite.\n";
	else cout<<n<<" is prime.\n";

}
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        vector<int> temp;
        queue<pair<TreeNode*, int> > q;
        q.push({root, 0});
        // temp.push(root->val);
        while(!q.empty()){
            TreeNode* curr = q.front().first;
            int dist = q.front().second;
            q.pop();
            if(curr) q.push({curr->left, dist+1});
            if(curr) q.push({curr->right, dist+1});
            if(curr) temp.push_back(curr->val);
            if(dist!=q.front().second){
                if(!temp.empty())ans.push_back(temp);
                temp.clear();
            }
        }
        for(int i=1; i<ans.size(); i=i+2){
            for(int j=0; j<ans[i].size()/2; j++){
                int temp = ans[i][j];
                ans[i][j] = ans[i][ans[i].size()-1-j];
                ans[i][ans[i].size()-1-j] = temp;
            }
        }
        return ans;
    }
};