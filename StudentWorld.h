#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    //constructor & destructor
    StudentWorld(string assetPath) : GameWorld(assetPath) {};
    ~StudentWorld() { cleanUp(); };
    
    //
    bool isBlockingObjectAt(int x, int y, Actor* a);
    bool isOverlap(Actor* a1, Actor* a2);
    void addActor(Actor* act) { actors.push_back(act); } ;
    
    //virtual functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //getters
    vector<Actor*>::iterator actorsBegin() { return actors.begin(); } ;
    vector<Actor*>::iterator actorsEnd() { return actors.end(); } ;
private:
    vector<Actor*> actors;
    vector<GameWorld*> g;
};

#endif // STUDENTWORLD_H_
