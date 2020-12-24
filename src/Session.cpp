#include <iostream>
#include <fstream>
#include "Agent.h"
#include "Session.h"
#include "Graph.h"
#include "json.hpp"
#include "vector"

using namespace std;
using json = nlohmann::json;

//============Read Json============
Session::Session(const std::string &path) : g({}), treeType(), agents(), numOfAgents(), currCycle(),
                                            Yellows() {
    std::ifstream i(path);
    json j;
    i >> j;

    g = Graph(j["graph"]);

    string tType = j["tree"];

    if (tType == "M")
        treeType = MaxRank;
    if (tType == "C")
        treeType = Cycle;
    if (tType == "R")
        treeType = Root;

    numOfAgents = 0;
    currCycle = 0;
    int size = g.getSize();
    for (int i = 0; i < size; i++) {
        Yellows.push_back(false);
    }
    for (auto &element :j["agents"]) {
        if (element[0] == "C") {
            auto *agent = new ContactTracer();
            agents.push_back(agent);
            numOfAgents++;
        } else {
            auto *agent = new Virus(element[1]);
            agents.push_back(agent);
            numOfAgents++;
            Yellows[element[1]]=true;
            this->updateStatus(element[1]);
        }
    }
}

//============Rule of 5============
//============Copy Constructor============
Session::Session(const Session &other) : g(other.g), treeType(), agents(vector<Agent *>()),
                                         numOfAgents(other.numOfAgents), currCycle(other.currCycle),
                                        Yellows(vector<bool>()) {
    g = other.g;
    treeType = other.treeType;
    for (auto curr: other.agents) {
        Agent *cloneAgent(curr);
        agents.push_back(cloneAgent->clone());
    }
    for (int i = 0; i < g.getSize(); i++) {
        Yellows[i] = other.Yellows[i];
    }
}

//============Destructor============
Session::~Session() {
    clear();
}


//============Copy Assignment============
Session &Session::operator=(const Session &session) {
    g = session.g;
    for (auto curr: session.agents)
        agents.push_back(curr);
    treeType = session.treeType;
    currCycle = session.currCycle;
    numOfAgents = session.numOfAgents;
    Yellows = session.Yellows;
    return *this;

}

//============Move Constructor============
Session::Session(Session &&other) : g(other.g), treeType(other.treeType), agents(vector<Agent *>()),
                                    numOfAgents(other.numOfAgents), currCycle(other.currCycle),
                                     Yellows(vector<bool>()) //
{
    for (auto curr: other.agents) {
        agents.push_back(curr);
    }
    for (auto curr: other.agents)
        delete curr;
    other.agents.clear();
    Yellows = other.Yellows;
    other.Yellows.clear();
}


//============Move Assignment Operator============
Session &Session::operator=(Session other) {
    if (this != &other) {
        this->clear();
        g = other.g;
        int size = agents.size();
        for (auto curr: other.agents)
            agents.push_back(curr);
        numOfAgents = other.numOfAgents;
        for (int i = 0; i < size; i++) { delete other.agents[i]; }
        treeType = other.treeType;
        currCycle = other.currCycle;
        int size2 = other.getGraph().getSize();
        for (int i = 0; i < size2; i++) {
            Yellows[i] = other.Yellows[i];
        }

    }
    return *this;
}

//============The Rest============
void Session::simulate() {
    int counterLoop = 1;
    int counter = 0;
    int check = this->getNumOfAgents() + 1;
    while ((!(this->finish())) | (check != this->getNumOfAgents())) {
        check = this->getNumOfAgents();
        for (Agent *curr: agents) {
            if (counter == check)
                break;
            counter++;
            curr->act(*this);
        }
        currCycle++;
        counter = 0;
        counterLoop++;
    }

    vector<bool> theWinnersBool = g.getStatus();
    vector<int> theWinners;
    int size = g.getSize();
    for (int i = 0; i < size; i++) {
        if (theWinnersBool[i]) {
            theWinners.push_back(i);
        }
    }
    nlohmann::json j;
    j["infected"] = theWinners;
    j["graph"] = g.getEdges();
    ofstream o("../bin/output.json");
    o << j;
    o.close();
}

void Session::addAgent(const Agent &agent) {
    Agent *newAgent = agent.clone();
    agents.push_back(newAgent);
    numOfAgents++;
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

void Session::enqueueInfected(int toInsert) {
    g.enqeue(toInsert);
}

int Session::dequeueInfected() {
    return g.deqeue();
}

TreeType Session::getTreeType() const {
    return treeType;
}

void Session::updateStatus(int node) {
    g.setStatus(node);
}

Graph Session::getGraph() {
    return g;
}

bool Session::finish() {
    int node;
    vector<bool> Stats = this->getGraph().getStatus();
    vector<int> neighboors;
    int graphSize = this->getGraph().getSize();
    queue<int> que;
    bool statusNode1, statusNode2;
    vector<int> marks(graphSize, 0);
    int currNeighboor;
    for (int i = 0; i < graphSize; i++) {
        if (marks[i] == 1)
            continue;
        statusNode1 = Stats[i];
        marks[i] = 1;
        node = i;
        que.push(node);
        bool isLonely = this->getGraph().getLonely(i);
        if (!isLonely) {// Start BFS on him .
            while (!que.empty()) {
                node = que.front();
                que.pop();
                neighboors = this->getGraph().neighboorOf(node);
                int neighboorSize = neighboors.size();
                for (int j = 0; j < neighboorSize; j++) {
                    currNeighboor = neighboors[j];
                    statusNode2 = Stats[currNeighboor];
                    if (statusNode1 != statusNode2)
                        return false;
                    if (marks[j] == 0) {
                        marks[j] = 1;
                        que.push(currNeighboor);
                    }
                }
            }
        }
    }
    return true;
}

const int Session::getCurrCycle() const {
    return currCycle;
}

int Session::getNumOfAgents() {
    return numOfAgents;
}


void Session::breakFromGraph(int toBreak) {
    g.breakFromGraph(toBreak);
}

void Session::clear() {
    for (Agent *i:agents)
        delete i;
    g.clear();
    agents.clear();
    Yellows.clear();
}



void Session::setYellow(int num, bool ans) {
    Yellows[num] = ans;
}




