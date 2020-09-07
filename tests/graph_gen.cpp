#include<bits/stdc++.h>
#include <unistd.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
#define FOR(i,a,b) 	for(int i=a;i<b;++i)
#define RFOR(i,a,b) 	for(int i=a;i>=b;--i)
#define ln 		"\n"
#define mp make_pair
#define pb push_back
#define sz(a)	ll(a.size())
#define F first
#define S second
#define all(c)	c.begin(),c.end()
#define rall(c) c.rbegin(), c.rend()
#define trace(c,x) for(auto &x:c)
#define pii pair<ll,ll>
#define init(a, x) memset(a,x,sizeof(a))
typedef long long ll;
typedef long double ld;
typedef	priority_queue<pii,std::vector<pii>,greater<pii> > revpr;

typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> pbds;
// ordered_set X
//K-th smallest
//*X.find_by_order(k-1)
//NO OF ELEMENTS < A
//X.order_of_key(A)
const int L=1e6+7;
map<ll,ll> counter;
ll fastexpo(ll x,ll y,ll m)
{
	ll temp=1;
	while(y>0)
	{
		if(y&1)temp = ((temp%m)*(x%m))%m;
		x = ((x%m)*(x%m))%m;
		y>>=1;
	}return temp;
}
void complete()
{
	int n = rand()% 100 + 1;
	cout << n <<" "<<(n*(n-1))/2<<ln;
	FOR(i,1,n+1)
	{
		FOR(j,i+1,n+1)cout << i <<" "<<j<<ln;
	}
}
void fff()
{
	int n = rand()% 100 + 1;
	int m = rand()%500 + 1;
	m = min(m, (n*(n-1))/2);
	if(rand()%4 == 0)
	{
		complete();
		return;
	}
	set<pii>SET;
	srand(time(0)); 

	for (int i = 0; i < m; ++i)
	{
		int a = rand()%n + 1;
		int b = rand()%n + 1;
		if(a==b||SET.find(make_pair(a,b))!=SET.end())continue;
		cout << a <<" "<<b<<ln;
	}
}
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	//int t;cin >> t;while(t--)
	fff();
	return 0;
}