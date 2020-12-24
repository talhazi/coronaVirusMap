#ifndef SESSION_H_
#define SESSION_H_
#include <vector>
#include <string>
#include "Graph.h"


class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);

    //============Rule of 5============
    Session(const Session &other);
    virtual ~Session();
    Session & operator=(const Session &other);
    Session(Session &&other);
    Session & operator=(Session other);


    //============the rest============
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    void enqueueInfected(int toInsert);
    int dequeueInfected();
    TreeType getTreeType() const;
    void updateStatus(int node);
    Graph getGraph();
    bool finish();
    const int getCurrCycle() const;
    int getNumOfAgents();
    void breakFromGraph(int toBreak);
    void clear();
    void setYellow (int num, bool ans);
private:
    Graph g;
    TreeType treeType;
    vector<Agent*> agents;
    int numOfAgents;
    int currCycle;
    vector<bool> Yellows;
};

#endif
