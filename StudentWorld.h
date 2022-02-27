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
    
    //actions
    bool isMovePossible(int x, int y, Actor* a) const;
    bool moveIfPossible(Actor* a, int destx, int desty) const;
    bool isOverlap(Actor* a1, Actor* a2) const;
    bool moveOrBonk(Actor *a, int destx, int desty) const;
    bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const;
    bool overlapsPeach(Actor* a) const;
    bool bonkOverlappingPeach(Actor* bonker) const;
    bool bonkOverlappingActor(Actor* bonker) const;
    bool damageOverlappingPeach(Actor* damager) const;
    
    // Record that a level was finished.  The parameter is true if it was
    // the last level.
    void endLevel(bool isGameWon);
    
    

    
    bool damageOverlappingActor(Actor* damager) const;
    

    //setters
    void setPeachHP(int hp) const { peach->setHP(hp); }
    void grantInvincibility(int ticks) const { peach->grantInvinc(ticks); }
    void grantShootPower() const { peach->setShoot(); }
    void grantJumpPower() const { peach->setJump(); }
    
    //modifiers
    void increaseScore(int x) { score += x; }
    void addActor(Actor* act) { actors.push_back(act); }
    
    //accessors
    bool isPeachInv() const {return peach->getInvinc(); }
    
    //virtual functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    

private:
    vector<Actor*> actors;
    Peach* peach;
    vector<GameWorld*> g;
    int score;
};

#endif // STUDENTWORLD_H_
