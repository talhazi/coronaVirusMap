#ifndef TREE_H_
#define TREE_H_
#include <queue>
#include <vector>
using namespace std;
class Session;

class Tree{
public:
    Tree(int rootLabel);
    //============Rule of 5============
    Tree(const Tree &other);
    virtual ~Tree();
    Tree &operator=(const Tree &other);
    Tree(Tree &&other);
    Tree &operator=(const Tree &&other);
    virtual Tree* clone() const=0;

    //============the rest============
    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    int getNum();
    Tree* getChild(int i);
    vector<Tree*> getChildren();
    void setChildren(Tree* toAdd);
    int getSize();
    virtual int traceTree()=0;


protected:
    int node;
    vector<Tree*> children;
};

//============Cycle Tree============
class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual CycleTree* clone() const;
    int getCycle();
    virtual int traceTree();




private:
    int currCycle;
};

//============MaxRank Tree============
class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual MaxRankTree* clone() const;
    int MaxDegree(Tree* Root);
    virtual int traceTree();


};

//============Root Tree============
class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual RootTree* clone() const;
};

#endif