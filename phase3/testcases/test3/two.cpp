unsigned long long large_num = 998244353;
#define sort_array(array) sort(array.begin(), array.end());
#define print_with_spaces(array, n) for (unsigned int i = 0; i < n; i++){cout << array[i] << " ";}
#define print_with_lines(array, n) for (unsigned int i = 0; i < n; i++){cout << array[i] << "\n";}
#define iterate(i,a,b) for (unsigned int i = a; i < b; i++)
#define yes cout << "YES\n"
#define no cout << "NO\n"
#define is_possible cout << ((possible) ? "YES\n": "NO\n")
#define vi vector<unsigned long long>
#define ull unsigned long long
#define ll long long
#define pb push_back
#define pi pair<unsigned long long, unsigned long long>
#define mp make_pair
#define read_array(array, n) for (unsigned int i = 0; i < n; i++){cin>>array[i];}

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

    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if(matrix.empty())
        {
            return 0;
        }
        int ans = -2147483647;
        int n = matrix[0].size();
        int m = matrix.size();
        // Iterate over all pairs of rows
        int i=0;
        while(i < m)
        {
            vector<int> colSum(n, 0);
            int j=0;
            while(j < m)
            {
                // Calculate the column sums for rows between i and j
                for(int k = 0; k < n; k++)
                {
                    colSum[k] += matrix[j][k];
                }
                // Find the maximum sum subarray no larger than k
                ans = max(ans, maxSubArraySumNoLargerThanK(colSum, k));
                j++;
            }
            i++;
        }
        return ans;
    }
    int maxSubArraySumNoLargerThanK(vector<int>& nums, int k) {
        int check = INT_MIN;
        set<int> prefixSums;
        prefixSums.insert(0);
        int add = 0;
        int i = 0;
        loop:
        // for(int num : nums)
        {
            int num = nums[i];
            add += num;
            auto it = prefixSums.lower_bound(add - k);
            if(it != prefixSums.end())
            {
                check = max(check, add - *it);
            }
            prefixSums.insert(add);
            i++;
            if(i<nums.size()) goto loop;
        }
        return check;
    }
};