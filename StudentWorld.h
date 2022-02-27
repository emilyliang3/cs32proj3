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
    StudentWorld(string assetPath) : GameWorld(assetPath), m_level(1), m_reachedFlag(false), m_reachedMario(false)  {};
    ~StudentWorld() { cleanUp(); };
    
    //actions
    //movement
    bool isMovePossible(int x, int y, Actor* a) const;
    bool moveIfPossible(Actor* a, int destx, int desty) const;
    bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const;
    bool moveOrBonk(Actor *a, int destx, int desty) const;
    bool isBlocking(int x, int y);
    
    //overlap
    bool isOverlap(Actor* a1, Actor* a2) const;
    bool overlapsPeach(Actor* a) const;
    bool bonkOverlappingPeach(Actor* bonker) const;
    bool bonkOverlappingActor(Actor* bonker) const;
    bool damageOverlappingPeach(Actor* damager) const;
    void peachBonkOverlap();
    bool damageOverlappingActor(Actor* damager) const;
    
    //helper
    void removeDeadActors();
    void endLevel(bool isGameWon);

    //setters
    void setPeachHP(int hp) const { peach->setHP(hp); }
    void grantInvincibility(int ticks) const { peach->grantInvinc(ticks);
        peach->setStar();
    }
    void grantShootPower() const { peach->setShoot(); }
    void grantJumpPower() const { peach->setJump(); }
    void grantStarPower() const { peach->setStar(); }
    
    //modifiers
    void addActor(Actor* act) { actors.push_back(act); }
    void addActorFront(Actor* act) { actors.insert(actors.begin(), act); }
    
    //accessors
    bool isPeachInv() const {return peach->hasStar(); }
    
    //virtual functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    

private:
    vector<Actor*> actors;
    Peach* peach;
    int m_level;
    bool m_reachedFlag;
    bool m_reachedMario;
};

#endif // STUDENTWORLD_H_
