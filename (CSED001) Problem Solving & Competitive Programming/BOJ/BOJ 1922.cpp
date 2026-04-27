#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

using ll=long long;

// [Study] class
class Edge{
    public:
        int vertex[2], distance;
        Edge(int a, int b, int c){
            vertex[0]=a; // [Study] this->vertex[0]=a;
            vertex[1]=b;
            distance=c;
        }
        bool operator <(Edge E){ // [Study] Edge E vs Edge &E
            return distance<E.distance;
        }
};

int parent[1000001], Rank[1000001]; // Union-by-Rank

int Find(int v){
    if(v==parent[v]){
        return v;
    }
    return parent[v]=Find(parent[v]); // Path compression
}

void Union(int u, int v){
    u=Find(u);
    v=Find(v);
    if(u==v){
        return;
    }
    if(Rank[u]==Rank[v]){
        parent[v]=u;
        Rank[u]++;
        return;
    }
    else if(Rank[u]>Rank[v]){
        parent[v]=u;
        return;
    }
    parent[u]=v;
    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, sum=0;
    vector<Edge> v;
    cin>>N;
    cin>>M;
    for(int i=1; i<=N; i++){
        parent[i]=i;
        Rank[i]=1;
    }
    for(int i=1; i<=M; i++){
        int a, b, c;
        cin>>a>>b>>c;
        v.push_back(Edge(a, b, c));
    }
    sort(v.begin(), v.end());
    for(int i=0; i<v.size(); i++){
        if(!(Find(v[i].vertex[0])==Find(v[i].vertex[1]))){
            sum+=v[i].distance;
            Union(v[i].vertex[0], v[i].vertex[1]);
        }
    }
    cout<<sum;
}