#ifndef GRAPH_H_
#define GRAPH_H_
#include <queue>
#include <iostream>
#include <vector>
#include <string>

class Tree;

class Session;

using namespace std;

class Graph {
public:
    Graph(vector<vector<int>> matrix);
    void breakFromGraph (int ind);
    void enqeue(int toInsert);
    int deqeue();
    vector<int> neighboorOf (int edge);
    bool isInfected(int nodeInd);
    void infectNode(int nodeInd);
    int getSize();
    vector<vector<int>> getEdges();
    vector<bool> getStatus();
    void removeEdge (int start, int end);
    Tree* BFS(Session &currSession,int rootLabel);
    void clear();
    void setStatus(int node);
    bool getLonely(int node);

private:
    static const int NULL_EDGE = 0;
    vector< vector<int> > edges;
    queue<int> infectedQueue;
    vector<bool> status;
    int numbVertices;

};

#endif