#pragma once
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

using ll=long long;

#define elif else if

#define MAX 26
#define INF 2147483647

struct TreeEdge{
    int node1;
    int node2;
    int weight;
};

class Graph{
    public:
        Graph(){
            edgeCount=0;
            for(int i(0); i<=MAX-1; ++i){
                nodeExists[i]=false;
                for(int j(0); j<=MAX-1; ++j){
                    adjMatrix[i][j]=0;
                    weightMatrix[i][j]=INF;
                }
            }
        }

        ~Graph(){};

        // Task 1. Undirected Graph - Count Simple Paths
        int addUndirectedEdge(string nodeA, string nodeB);
        string countSimplePaths();

        // Task 2. Directed Graph - Semester Scheduling
        int addDirectedEdge(string nodeA, string nodeB);
        string getTopologicalSort();

        // Task 3. Directed Graph - Strongly & Weakly Connected Components
        string getStronglyConnectedComponents();
        string getWeaklyConnectedComponents();

        // Task 4. Single Source Shortest Path - Dijkstra's Algorithm
        int addDirectedEdge(string nodeA, string nodeB, int weight);
        string dijkstra(string source, int budget, ofstream &fout);

        // Task 5. Maximum-Cost Spanning Tree - Second-Best Tree
        int addUndirectedEdge(string nodeA, string nodeB, int weight);
        ll findMaST(ofstream &fout);

    private:
        int adjMatrix[MAX][MAX];
        int weightMatrix[MAX][MAX];
        bool nodeExists[MAX];
        TreeEdge weightedEdges[MAX*MAX];
        int edgeCount;

        ll countSimplePaths(int node, bool visited[]);

        void visitReachableNodes(int node, bool visited[]);

        int findParent(int parent[], int node);
        void unionParent(int parent[], int node1, int node2);
        bool isSameParent(int parent[], int node1, int node2);
        bool comesBefore(TreeEdge edge1, TreeEdge edge2);
        bool isSameEdge(TreeEdge edge1, TreeEdge edge2);
        void sortTreeEdges(TreeEdge edges[], int size);
        ll findMaximumSpanningTree(TreeEdge selectedEdges[]);
        bool findPath(int node, int target, bool visited[], TreeEdge selectedEdges[], int selectedSize, int path[], int &pathSize);
};