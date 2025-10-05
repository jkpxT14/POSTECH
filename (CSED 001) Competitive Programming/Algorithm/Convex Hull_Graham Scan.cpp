// Baekjoon OJ 1708

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

using ll=long long;

class Point{
    public:
        ll x, y;
        bool operator <(const Point &p){
            return y==p.y ? x<p.x : y<p.y;
        }
};

int N;
Point P[100001];
stack<int> st;

ll ccw(const Point &p, const Point &q, const Point &r) // Counter Clock Wise = 반시계 방향
{
    return p.x*q.y + q.x*r.y + r.x*p.y - q.x*p.y - r.x*q.y - p.x*r.y;
}

ll dist(Point p, Point q)
{
    return (p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y);
}

bool cmp(const Point &p, const Point &q)
{
    ll c=ccw(P[0], p, q);
    if(c==0){
        return dist(P[0], p)<dist(P[0], q);
    }
    return c>0;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>N;
    for(int i=0; i<N; i++){
        cin>>P[i].x>>P[i].y;
    }

    sort(P, P+N);
    sort(P+1, P+N, cmp);

    st.push(0);
    st.push(1);
    for(int i=2; i<N; i++){
        while(st.size()>=2){
            int first, second;
            first=st.top();
            st.pop();
            second=st.top();
            if(ccw(P[i], P[second], P[first])>0){
                st.push(first);
                break;
            }
        }
        st.push(i);
    }

    cout<<st.size();
}