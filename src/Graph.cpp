#include "Graph.h"
#include "Session.h"
#include "Tree.h"

using namespace std;

Graph::Graph(vector<vector<int>> matrix) : edges(vector<vector<int>>()), infectedQueue(queue<int>()),
                                           status(vector<bool>()), numbVertices(matrix.size()) {
    int size = matrix.size();
    for (int i = 0; i < size; i++) {
        edges.push_back(matrix[i]);
    }
    for (int i = 0; i < numbVertices; i++) {
        status.push_back(false);
    }
}

void Graph::breakFromGraph(int ind) {
    int size = this->numbVertices;
    for (int i = 0; i < size; i++) {
        removeEdge(i, ind);
    }
}

void Graph::enqeue(int toInsert) {
    infectedQueue.push(toInsert);
}

int Graph::deqeue() {
    if (infectedQueue.empty())
        return -2;
    int toReturn = infectedQueue.front();
    infectedQueue.pop();
    return toReturn;
}

vector<int> Graph::neighboorOf(int edge) {
    vector<int> theNeighboors;
    vector<int> x = edges.at(edge);

    for (int i = 0; i < numbVertices; i++) {
        if (x[i] == 1)
            theNeighboors.push_back(i);
    }
    return theNeighboors;
}

bool Graph::isInfected(int nodeInd) {
    return (status[nodeInd]);
}

void Graph::infectNode(int nodeInd) {
    status[nodeInd] = true;
}

int Graph::getSize() {
    return numbVertices;
}

vector<vector<int>> Graph::getEdges() {
    return edges;
}

vector<bool> Graph::getStatus() {
    return status;
}

void Graph::removeEdge(int start, int end) {
    edges[start][end] = 0;
    edges[end][start] = 0;
}

Tree *Graph::BFS(Session &currSession, int rootLabel) {
    int newLabelTree;
    vector<int> neighboors;
    Tree *rootptr = Tree::createTree(currSession, rootLabel);
    int graphSize = currSession.getGraph().getSize();
    queue<int> que;
    vector<Tree *> IndexTree(graphSize, nullptr);
    vector<int> marks(graphSize, 0);
    marks[rootLabel] = 1; // visited marks.
    IndexTree[rootLabel] = rootptr;// Tree* vector
    que.push(rootLabel);//queue of labels;
    Tree *currNeighboor;

    while (!que.empty()) {
        newLabelTree = que.front();
        que.pop();
        Tree *currTree = IndexTree[newLabelTree];
        neighboors = currSession.getGraph().neighboorOf(newLabelTree);
        int size = neighboors.size();
        for (int j = 0; j < size; j++) {
            if (marks[neighboors[j]] == 0) {
                marks[neighboors[j]] = 1;
                currNeighboor = Tree::createTree(currSession, neighboors[j]);
                currTree->setChildren(currNeighboor);
                que.push(neighboors[j]);
                IndexTree[neighboors[j]] = currNeighboor;
            }
        }

    }
    return IndexTree[rootLabel];
}


void Graph::clear() {
    for (int i = 0; i < numbVertices; i++)
        edges[i].clear();
}


void Graph::setStatus(int node) {
    status[node] = true;
}

bool Graph::getLonely(int node) {
    for (int i = 0; i < numbVertices; i++) {
        if (edges[node][i] == 1)
            return false;
    }
    return true;
}






