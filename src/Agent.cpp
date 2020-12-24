#include <iostream>
#include "Agent.h"
#include "Tree.h"
#include "Session.h"


using namespace std;

Agent::Agent() {}

//============ContactTracer============
ContactTracer::ContactTracer() {}

ContactTracer *ContactTracer::clone() const {
    return new ContactTracer(*this);
}

void ContactTracer::act(Session &session) {

    int theRoot = session.dequeueInfected();
    if (theRoot != -2) {
        Tree *BFSTree = session.getGraph().BFS(session, theRoot);
        int toBreak = BFSTree->traceTree();
        delete BFSTree;

        session.breakFromGraph(toBreak);
    }
}


//============Virus============
Virus::Virus(int nodeInd) : nodeInd(nodeInd), firstTime(true) {}

Virus *Virus::clone() const {
    return new Virus(*this);
}

void Virus::act(Session &session) {

    if (this->firstTime) // First time we landing on this Agent
    {
            session.setYellow(nodeInd, false);//this is red from now.
            this->firstTime = false;
             session.enqueueInfected(this->nodeInd);
    }
    bool currNode;
    vector<int> neighborsOfNode = session.getGraph().neighboorOf(nodeInd); //get sorted vector from min to max ind
    int size = neighborsOfNode.size();
    for (int i = 0; i < size; i++) {
        currNode = session.getGraph().isInfected(neighborsOfNode[i]);
        if (!(currNode)) { //means currNode isn't sick yet.
            Virus spreadTo(neighborsOfNode[i]);
            session.addAgent(spreadTo);   //add the node that got infected to agent
            session.setYellow(nodeInd, true);
            session.updateStatus(neighborsOfNode[i]);
            break;
        }
    }
}


