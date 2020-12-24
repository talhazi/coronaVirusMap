#ifndef AGENT_H_
#define AGENT_H_
#include <vector>

class Session;

class Agent{
public:
    Agent();
    virtual ~Agent()=default;
    virtual Agent* clone() const=0;
    virtual void act(Session& session)=0;


};


//============ContactTracer============
class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session& session) ;
    virtual ContactTracer* clone() const;


};


//============Virus============
class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual void act(Session& session);
    virtual Virus* clone() const;


private:
    const int nodeInd;
    bool firstTime;
};

#endif
