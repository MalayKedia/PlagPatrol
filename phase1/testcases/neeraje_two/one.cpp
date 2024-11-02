#include<bits/stdc++.h>
using namespace std;

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    unsigned long test_cases; cin>>test_cases;
    for (unsigned long p = 0; p < test_cases; p++){
        unsigned int n; cin>>n;
        vector<ull> array(n);
        read_array(array, n);
        sort_array(array);
        if (array[0] + array[1] > array[n-1]){
            cout << 0 << "\n";
            continue;
        }
        vector<ull> sums{};
        for (int i = 1; i < n; i++){
            sums.push_back(array[i] + array[i-1]);
        }
        int minimum=INT_MAX;
        for (unsigned int i = 0; i < n; i++){
            int potential_maximum=array[i];
            int count=0;
            // cout << upper_bound(sums.begin(),sums.end(),potential_maximum)-sums.begin() << " ";
            // cout << array.end()-upper_bound(array.begin(),array.end(),potential_maximum) << "\n";
            count+=upper_bound(sums.begin(),sums.end(),potential_maximum)-sums.begin();
            count+=array.end()-upper_bound(array.begin(),array.end(),potential_maximum);
            minimum=min(minimum,count);
        }
        cout << minimum << "\n";
    }
}