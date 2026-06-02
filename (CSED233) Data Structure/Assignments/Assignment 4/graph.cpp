#include "graph.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

using ll=long long;

#define elif else if

#define INF 2147483647

int getIndex(string node){
    return node[0]-'A';
}

// Task 1. Undirected Graph - Count Simple Paths
int Graph::addUndirectedEdge(string nodeA, string nodeB){
    int node1(getIndex(nodeA)), node2(getIndex(nodeB));
    nodeExists[node1]=true; nodeExists[node2]=true;
    if(adjMatrix[node1][node2]==1){
        return 0;
    }
    adjMatrix[node1][node2]=1;
    adjMatrix[node2][node1]=1;
    return 0;
}

string Graph::countSimplePaths(){
    bool visited[MAX]={false,};
    return to_string(countSimplePaths(0, visited));
}

ll Graph::countSimplePaths(int node, bool visited[]){
    if(node==MAX-1){
        return 1;
    }
    ll count(0);
    visited[node]=true;
    for(int i(0); i<=MAX-1; ++i){
        if(adjMatrix[node][i]==1 && !visited[i]){
            count+=countSimplePaths(i, visited);
        }
    }
    visited[node]=false;
    return count;
}

// Task 2. Directed Graph - Semester Scheduling
int Graph::addDirectedEdge(string nodeA, string nodeB){
    int node1(getIndex(nodeA)), node2(getIndex(nodeB));
    nodeExists[node1]=true; nodeExists[node2]=true;
    if(adjMatrix[node1][node2]==1){
        return 0;
    }
    adjMatrix[node1][node2]=1;
    return 0;
}

string Graph::getTopologicalSort(){
    int remainingGraph[MAX][MAX];
    int indegree[MAX];
    bool scheduled[MAX];
    int totalNodes(0);
    int scheduledNodes(0);
    string answer("");
    for(int i(0); i<=MAX-1; ++i){
        scheduled[i]=false;
        if(nodeExists[i]){
            ++totalNodes;
        }
        for(int j(0); j<=MAX-1; ++j){
            remainingGraph[i][j]=adjMatrix[i][j];
        }
    }
    while(scheduledNodes<totalNodes){
        for(int i(0); i<=MAX-1; ++i){
            indegree[i]=0;
        }
        for(int i(0); i<=MAX-1; ++i){
            if(!nodeExists[i] || scheduled[i]){
                continue;
            }
            for(int j(0); j<=MAX-1; ++j){
                if(nodeExists[j] && !scheduled[j] && remainingGraph[j][i]==1){
                    ++indegree[i];
                }
            }
        }
        int semester[MAX];
        int semesterSize(0);
        for(int i(0); i<=MAX-1; ++i){
            if(nodeExists[i] && !scheduled[i] && indegree[i]==0){
                semester[semesterSize++]=i;
            }
        }
        if(semesterSize==0){
            return "ERROR";
        }
        if(answer.length()>=1){
            answer+=" | ";
        }
        for(int i(0); i<=semesterSize-1; ++i){
            if(i>=1){
                answer+=" ";
            }
            answer+=char(semester[i]+'A');
        }
        for(int i(0); i<=semesterSize-1; ++i){
            scheduled[semester[i]]=true;
            ++scheduledNodes;
            for(int j(0); j<=MAX-1; ++j){
                remainingGraph[semester[i]][j]=0;
            }
        }
    }
    return answer;
}

// Task 3. Directed Graph - Strongly & Weakly Connected Components
void Graph::visitReachableNodes(int node, bool visited[]){
    visited[node]=true;
    for(int i(0); i<=MAX-1; ++i){
        if(nodeExists[i] && adjMatrix[node][i]==1 && !visited[i]){
            visitReachableNodes(i, visited);
        }
    }
}

string Graph::getStronglyConnectedComponents(){
    bool reachable[MAX][MAX];
    bool visited[MAX];
    bool checked[MAX];
    string answer("");
    for(int i(0); i<=MAX-1; ++i){
        checked[i]=false;
        for(int j(0); j<=MAX-1; ++j){
            reachable[i][j]=false;
        }
    }
    for(int i(0); i<=MAX-1; ++i){
        if(!nodeExists[i]){
            continue;
        }
        for(int j(0); j<=MAX-1; ++j){
            visited[j]=false;
        }
        visitReachableNodes(i, visited);
        for(int j(0); j<=MAX-1; ++j){
            reachable[i][j]=visited[j];
        }
    }
    for(int i(0); i<=MAX-1; ++i){
        if(!nodeExists[i] || checked[i]){
            continue;
        }
        if(answer.length()>=1){
            answer+="\n";
        }
        bool first(true);
        for(int j(0); j<=MAX-1; ++j){
            if(nodeExists[j] && reachable[i][j] && reachable[j][i]){
                checked[j]=true;
                if(!first){
                    answer+=" ";
                }
                answer+=char(j+'A');
                first=false;
            }
        }
    }
    return answer;
}

string Graph::getWeaklyConnectedComponents(){
    bool visited[MAX];
    bool checked[MAX];
    string answer("");
    for(int i(0); i<=MAX-1; ++i){
        checked[i]=false;
    }
    for(int i(0); i<=MAX-1; ++i){
        if(!nodeExists[i] || checked[i]){
            continue;
        }
        for(int j(0); j<=MAX-1; ++j){
            visited[j]=false;
        }
        visitReachableNodes(i, visited);
        if(answer.length()>=1){
            answer+="\n";
        }
        bool first(true);
        for(int j(0); j<=MAX-1; ++j){
            if(nodeExists[j] && visited[j]){
                checked[j]=true;
                if(!first){
                    answer+=" ";
                }
                answer+=char(j+'A');
                first=false;
            }
        }
    }
    return answer;
}

// Task 4. Single Source Shortest Path - Dijkstra's Algorithm
int Graph::addDirectedEdge(string nodeA, string nodeB, int weight){
    int node1(getIndex(nodeA)), node2(getIndex(nodeB));
    nodeExists[node1]=true; nodeExists[node2]=true;
    weightMatrix[node1][node2]=weight;
    return 0;
}

string Graph::dijkstra(string source, int budget, ofstream &fout){
    int start(getIndex(source));
    const ll INF_DISTANCE(9223372036854775807LL/4);
    ll dist[MAX];
    int parent[MAX];
    int pathNodeCount[MAX];
    bool finalized[MAX];
    for(int i(0); i<=MAX-1; ++i){
        dist[i]=INF_DISTANCE;
        parent[i]=-1;
        pathNodeCount[i]=0;
        finalized[i]=false;
    }
    nodeExists[start]=true;
    dist[start]=0;
    pathNodeCount[start]=1;
    for(int i(0); i<=MAX-1; ++i){
        int currentNode(-1);
        for(int j(0); j<=MAX-1; ++j){
            if(nodeExists[j] && !finalized[j] && dist[j]!=INF_DISTANCE){
                if(currentNode==-1 || dist[j]<dist[currentNode]){
                    currentNode=j;
                }
            }
        }
        if(currentNode==-1){
            break;
        }
        finalized[currentNode]=true;
        for(int j(0); j<=MAX-1; ++j){
            if(!nodeExists[j] || finalized[j] || weightMatrix[currentNode][j]==INF){
                continue;
            }
            ll newDist(dist[currentNode]+weightMatrix[currentNode][j]);
            int newNodeCount(pathNodeCount[currentNode]+1);
            if(newDist<dist[j]){
                dist[j]=newDist;
                parent[j]=currentNode;
                pathNodeCount[j]=newNodeCount;
            } elif(newDist==dist[j] && newNodeCount>pathNodeCount[j]){
                parent[j]=currentNode;
                pathNodeCount[j]=newNodeCount;
            }
        }
    }
    bool hasPath(false);
    int bestDestination(-1);
    for(int i(0); i<=MAX-1; ++i){
        if(i==start){
            continue;
        }
        if(nodeExists[i] && dist[i]!=INF_DISTANCE && dist[i]<=budget){
            fout<<char(i+'A')<<" "<<dist[i]<<endl;
            hasPath=true;
            if(bestDestination==-1 || pathNodeCount[i]>pathNodeCount[bestDestination]){
                bestDestination=i;
            }
        }
    }
    if(!hasPath){
        fout<<endl;
        return "";
    }
    int path[MAX];
    int pathSize(0);
    int current(bestDestination);
    while(current!=-1){
        path[pathSize++]=current;
        current=parent[current];
    }
    for(int i(pathSize-1); i>=0; --i){
        fout<<char(path[i]+'A')<<" ";
    }
    fout<<dist[bestDestination]<<endl;
    return "";
}

// Task 5. Maximum-Cost Spanning Tree - Second-Best Tree
int Graph::addUndirectedEdge(string nodeA, string nodeB, int weight){
    int node1(getIndex(nodeA)), node2(getIndex(nodeB));
    if(node2<node1){
        int temp(node1);
        node1=node2;
        node2=temp;
    }
    nodeExists[node1]=true; nodeExists[node2]=true;
    weightMatrix[node1][node2]=weight;
    weightMatrix[node2][node1]=weight;
    weightedEdges[edgeCount].node1=node1;
    weightedEdges[edgeCount].node2=node2;
    weightedEdges[edgeCount].weight=weight;
    ++edgeCount;
    return 0;
}

int Graph::findParent(int parent[], int node){
    if(parent[node]==node){
        return node;
    }
    return parent[node]=findParent(parent, parent[node]);
}

void Graph::unionParent(int parent[], int node1, int node2){
    int parent1(findParent(parent, node1)), parent2(findParent(parent, node2));
    if(parent1<parent2){
        parent[parent2]=parent1;
    } else{
        parent[parent1]=parent2;
    }
}

bool Graph::isSameParent(int parent[], int node1, int node2){
    return findParent(parent, node1)==findParent(parent, node2);
}

bool Graph::comesBefore(TreeEdge edge1, TreeEdge edge2){
    if(edge1.weight!=edge2.weight){
        return edge1.weight>edge2.weight;
    }
    if(edge1.node1!=edge2.node1){
        return edge1.node1<edge2.node1;
    }
    return edge1.node2<edge2.node2;
}

bool Graph::isSameEdge(TreeEdge edge1, TreeEdge edge2){
    return edge1.node1==edge2.node1 && edge1.node2==edge2.node2 && edge1.weight==edge2.weight;
}

void Graph::sortTreeEdges(TreeEdge edges[], int size){
    for(int i(0); i<=size-2; ++i){
        for(int j(i+1); j<=size-1; ++j){
            if(!comesBefore(edges[i], edges[j])){
                TreeEdge temp(edges[i]);
                edges[i]=edges[j];
                edges[j]=temp;
            }
        }
    }
}

ll Graph::findMaximumSpanningTree(TreeEdge selectedEdges[]){
    int parent[MAX];
    int vertexCount(0);
    int selectedCount(0);
    ll totalCost(0);
    for(int i(0); i<=MAX-1; ++i){
        parent[i]=i;
        if(nodeExists[i]){
            ++vertexCount;
        }
    }
    for(int i(0); i<=edgeCount-1; ++i){
        int node1(weightedEdges[i].node1), node2(weightedEdges[i].node2);
        if(!isSameParent(parent, node1, node2)){
            unionParent(parent, node1, node2);
            selectedEdges[selectedCount++]=weightedEdges[i];
            totalCost+=weightedEdges[i].weight;
            if(selectedCount==vertexCount-1){
                break;
            }
        }
    }
    if(selectedCount!=vertexCount-1){
        return -1;
    }
    return totalCost;
}

bool Graph::findPath(int node, int target, bool visited[], TreeEdge selectedEdges[], int selectedSize, int path[], int &pathSize){
    visited[node]=true;
    if(node==target){
        return true;
    }
    for(int i(0); i<=selectedSize-1; ++i){
        int next(-1);
        if(selectedEdges[i].node1==node){
            next=selectedEdges[i].node2;
        } elif(selectedEdges[i].node2==node){
            next=selectedEdges[i].node1;
        }
        if(next!=-1 && !visited[next]){
            path[pathSize++]=i;
            if(findPath(next, target, visited, selectedEdges, selectedSize, path, pathSize)){
                return true;
            }
            --pathSize;
        }
    }
    return false;
}

ll Graph::findMaST(ofstream &fout){
    TreeEdge maxTree[MAX];
    TreeEdge bestTree[MAX];
    int vertexCount(0);
    for(int i(0); i<=MAX-1; ++i){
        if(nodeExists[i]){
            ++vertexCount;
        }
    }
    int treeSize(vertexCount-1);
    sortTreeEdges(weightedEdges, edgeCount);
    ll maxCost(findMaximumSpanningTree(maxTree));
    if(maxCost==-1){
        return -1;
    }
    ll bestCost(-1);
    for(int i(0); i<=edgeCount-1; ++i){
        bool isTreeEdge(false);
        for(int j(0); j<=treeSize-1; ++j){
            if(isSameEdge(weightedEdges[i], maxTree[j])){
                isTreeEdge=true;
                break;
            }
        }
        if(isTreeEdge){
            continue;
        }
        bool visited[MAX]={false,};
        int path[MAX];
        int pathSize(0);
        findPath(weightedEdges[i].node1, weightedEdges[i].node2, visited, maxTree, treeSize, path, pathSize);
        int removedEdgeIndex(-1);
        int removedEdgeWeight(INF);
        for(int j(0); j<=pathSize-1; ++j){
            int index(path[j]);
            if(maxTree[index].weight>weightedEdges[i].weight && maxTree[index].weight<removedEdgeWeight){
                removedEdgeWeight=maxTree[index].weight;
                removedEdgeIndex=index;
            }
        }
        if(removedEdgeIndex==-1){
            continue;
        }
        ll newCost(maxCost-maxTree[removedEdgeIndex].weight+weightedEdges[i].weight);
        if(newCost>bestCost && newCost<maxCost){
            bestCost=newCost;
            int bestTreeSize(0);
            for(int j(0); j<=treeSize-1; ++j){
                if(j!=removedEdgeIndex){
                    bestTree[bestTreeSize++]=maxTree[j];
                }
            }
            bestTree[bestTreeSize++]=weightedEdges[i];
        }
    }
    if(bestCost==-1){
        return -1;
    }
    sortTreeEdges(bestTree, treeSize);
    for(int i(0); i<=treeSize-1; ++i){
        fout<<char(bestTree[i].node1+'A')<<" "<<char(bestTree[i].node2+'A')<<" "<<bestTree[i].weight<<endl;
    }
    return bestCost;
}