#include "Tree.h"
#include "Session.h"
#include <queue>

class Tree;

Tree::Tree(int rootLabel) : node(rootLabel), children(vector<Tree *>()) {};

//============Rule of 5============
//============Copy Constructor============
Tree::Tree(const Tree &other) : node(other.node), children(vector<Tree *>()) {
    int size = other.children.size();
    for (int i = 0; i < size; i++) { children.push_back(other.children[i]); }
}

//============Destructor============
Tree::~Tree() {
    int size = children.size();
    for (int i = 0; i < size; i++) {
        if (children[i])
            delete children[i];
    }
    children.clear();
}

//============Copy Assignment============
Tree &Tree::operator=(const Tree &other) {
    node = other.node;
    int size = other.children.size();
    for (int i = 0; i < size; i++) { children.push_back(other.children[i]); }
    return *this;
}

//============Move Constructor============
Tree::Tree(Tree &&other) : node(other.node), children(vector<Tree *>()) {
    int size = other.children.size();
    for (int i = 0; i < size; i++) {
        children.push_back(other.children[i]);
        delete other.children[i];
    }
    other.children.resize(0);
    other.node = -1;
}

//============Move Assignment Operator============
Tree &Tree::operator=(const Tree &&other) {
    if (this != &other) {
        node = other.node;
        int size = other.children.size();
        for (int i = 0; i < size; i++) {
            children.push_back(other.children[i]);
            delete other.children[i];
        }
    }
    return *this;
}

//============The Rest============
void Tree::addChild(const Tree &child) {
    Tree *clone = child.clone();
    children.push_back(clone);
}

Tree *Tree::createTree(const Session &session, int rootLabel) {
    TreeType type = session.getTreeType();
    Tree *TreeRoot;
    if (type == Root)
        TreeRoot = new RootTree(rootLabel);
    else if (session.getTreeType() == MaxRank)
        TreeRoot = new MaxRankTree(rootLabel);
    else
        TreeRoot = new CycleTree(rootLabel, session.getCurrCycle());
    return TreeRoot;
}

int Tree::getNum() {
    return node;
}

Tree *Tree::getChild(int i) {
    return children[i];
}

vector<Tree *> Tree::getChildren() {
    return children;
}

void Tree::setChildren(Tree *toAdd) {
    children.push_back(toAdd);
}

int Tree::getSize() {
    return children.size();
}


//============Cycle Tree============
CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel), currCycle(currCycle) {}

CycleTree *CycleTree::clone() const {
    return new CycleTree(*this);
}

int CycleTree::getCycle() {
    return currCycle;
}

int CycleTree::traceTree() {
    Tree *ptr = this;
    int check = currCycle;
    for (int i = 0; i < check; i++) {
        if (ptr->getChildren().empty())
            break;
        ptr = ptr->getChild(0);
    }
    return ptr->getNum();
}


//============MaxRank Tree============
MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {}

MaxRankTree *MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}


int MaxRankTree::MaxDegree(Tree *Root) {
    Tree *CopyNode(Root);
    Tree *CopyNode2(Root);
    int labelToReturn = CopyNode->getNum();
    queue<Tree *> que;
    que.push(CopyNode);
    int size = CopyNode->getSize();
    if (size != 0)
        while (!que.empty()) {
            CopyNode2 = que.front();
            que.pop();
            for (int i = 0; i < CopyNode2->getSize(); i++) {
                CopyNode = CopyNode2->getChild(i);
                que.push(CopyNode);
                if (CopyNode->getSize() > size) {
                    size = CopyNode->getSize();
                    labelToReturn = CopyNode->getNum();
                    continue;
                }
            }
        }
    return labelToReturn;
}

int MaxRankTree::traceTree() {
    return MaxDegree(this);
}


//============Root Tree============
RootTree::RootTree(int rootLabel) : Tree(rootLabel) {}

RootTree *RootTree::clone() const {
    return new RootTree(*this);
}

int RootTree::traceTree() {
    return (this->getNum());
}
