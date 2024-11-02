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
        vector<ll> array(n);
        read_array(array, n);
        if (n==1){
            cout << 0 << "\n";
            continue;
        }
        vector<long long> values(n, 0);
        int prev=0;
        int next;
        for (int i = 1; i < n; i++){
            next=(prev+2)%n;
            ll edge_value=values[prev];
            ll first_end=array[(prev+1)%n];
            ll second_start=array[next];
            values[next]=edge_value+first_end-second_start;
            prev=next;
        }
        vector<ll> final_values(n);
        ll sum=0;
        for (unsigned int i = 0; i < n; i++){
            if (i%2 == 0){
                sum-=values[i];
            }
            else {
                sum+=values[i];
            }
        }
        ll x=sum;
        final_values[0]=0;
        prev=0;
        for (unsigned int i = 1; i < n; i++){
            final_values[i]=x+values[i-1]-prev;
            prev=final_values[i];
        }
        ll minimum=LLONG_MAX;
        for (int i = 0; i < n; i++){
            minimum=min(minimum,final_values[i]);
        }
        for (int i = 0; i < n; i++){
            cout << final_values[i]-minimum << " ";
        }
        cout << "\n";
    }
}