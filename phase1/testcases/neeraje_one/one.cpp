#include <bits/stdc++.h>
using namespace std;
#define sort(v) sort((v).begin(), (v).end())
#define search(v,e) binary_search(v.begin(),v.end(),e)
#define reverse(v) reverse((v).begin(), (v).end())
#define out(x) cout<<x<<endl
#define sz(x) (int) x.size()
//arr and loops
#define int long long
#define float double
#define fun1 function<void(int,int)>
#define fun2 function<int(int,int)>
#define fun3 function<int(int,int,int)>
#define f(i,a,b) for (int i=(a); i<=(b); ++i)
#define rof(i,a,b) for(int i=b;i>=a;i--)
#define mod 1000000007
int dx[8]={1,0,-1,0,1,1,-1,-1};
int dy[8]={0,1,0,-1,1,-1,1,-1};
//pair
#define pi pair<int,int>
#define pc pair<char,char>
#define fs first
#define ss second
// vector
#define vi vector<int>
#define vvi vector<vector<int>>
#define vc vector<char>
#define vvc vector<vector<char>>
#define vpi vector<pair<int,int>>
#define vvpi vector<vector<pair<int,int>>>
#define pb emplace_back
#define mp make_pair 
// queue
#define qi queue<int>
#define dqi deque<int>
// map
#define mi map<int,int>
#define si set<int>
#define msi multiset<int>
void io(){
#ifndef ONLINE_JUDGE
freopen("input.txt","r",stdin); 
freopen("ouput.txt","w",stdout);
#endif
}


void solve(){
int n; cin>>n;
vi a(n);
f(i,0,n-1) cin>>a[i];
if(n==1) {out(0); return;
}

vi values(n,0);
int prev=0;
int next;
f(i,0,n-1){
    next=(prev+2)%n;
            int edge_value=values[prev];
            int first_end=a[(prev+1)%n];
            int second_start=a[next];
            values[next]=edge_value+first_end-second_start;
            prev=next;


}
vi final(n);
int sum=0;
f(i,0,n-1){
    if(i%2==0) sum-=values[i];
    else sum+=values[i];
}
int x=sum;
final[0]=0;
prev=0;
f(i,1,n-1){
    final[i]=x+values[i-1]-prev;
    prev=final[i];
}

int mini= 1e18;
f(i,0,n-1) mini=min(mini,final[i]);
f(i,0,n-1) cout<<final[i]-mini<<' ';
cout<<endl;

}

signed main() {
io();
ios_base::sync_with_stdio(0);
cin.tie(0); cout.tie(0);
int t=1;
cin>>t;

{while(t--){
solve();}}

}