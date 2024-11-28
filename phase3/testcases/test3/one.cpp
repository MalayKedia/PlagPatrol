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

    long long unsigned int fac(int n){   //to calculate huge factorials

        if(n==0) return 1;      //base case fac(0)=1
        return n*fac(n-1);      //recursive definition of factorial
    }

    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        if(matrix.empty())
        {
            return 0;
        }
        int ans = INT_MIN;
        int m = matrix.size();
        int n = matrix[0].size();
        // Iterate over all pairs of rows
        for(int i = 0; i < m; i++)
        {
            vector<int> colSum(n, 0);
            for(int j = i; j < m; j++)
            {
                // Calculate the column sums for rows between i and j
                for(int k = 0; k < n; k++)
                {
                    colSum[k] += matrix[j][k];
                }
                // Find the maximum sum subarray no larger than k
                ans = max(ans, maxSubArraySumNoLargerThanK(colSum, k));
            }
        }
        return ans;
    }
    int maxSubArraySumNoLargerThanK(vector<int>& nums, int k) {
        int check = INT_MIN;
        set<int> prefixSums;
        prefixSums.insert(0);
        int add = 0;
        for(int num : nums)
        {
            add += num;
            auto it = prefixSums.lower_bound(add - k);
            if(it != prefixSums.end())
            {
                check = max(check, add - *it);
            }
            prefixSums.insert(add);
        }
        return check;
    }
};