#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

//**********************
//movement functions
//**********************

//returns true if possible for a to move x,y; false otherwise
bool StudentWorld::isMovePossible(int x, int y, Actor* a) const
{
    vector<Actor*>::const_iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        Actor* a2 = *it;
        if (a2->isSolid()) {
            if (x+SPRITE_WIDTH-1 >= a2->getX() && x <= a2->getX()+SPRITE_WIDTH-1 && y+SPRITE_HEIGHT-1 >= a2->getY() && y <= a2->getY()+SPRITE_HEIGHT-1)
                return false;
        }
    }
    return true;
}

//returns true if a solid object is blocking (x,y); false otherwise
bool StudentWorld::isBlocking(int x, int y)
{
    vector<Actor*>::const_iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        Actor* a2 = *it;
        if (a2->isSolid()) {
            if (x >= a2->getX() && x <= a2->getX()+SPRITE_WIDTH-1 && y >= a2->getY() && y <= a2->getY()+SPRITE_HEIGHT-1)
                return true;
        }
    }
    return false;
}

//returns true if a can move to x,y and moves it; false otherwise
bool StudentWorld::moveIfPossible(Actor* a, int x, int y) const
{
    if (!isMovePossible(x, y, a))
        return false;
    a->moveTo(x, y);
    return true;
}

//returns true if a can move to destx, desty and moves it; otherwise bonk object blocking destx,desty and return false
bool StudentWorld::moveOrBonk(Actor *a, int destx, int desty) const
{
    vector<Actor*>::const_iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        Actor* a2 = *it;
        if (a2->isSolid()) {
            if (destx+SPRITE_WIDTH-1 >= a2->getX() && destx <= a2->getX()+SPRITE_WIDTH-1 && desty+SPRITE_WIDTH-1 >= a2->getY() && desty <= a2->getY()+SPRITE_HEIGHT-1) {
                a2->getBonked(false);
                return false;
            }
        }
    }
    a->moveTo(destx, desty);
    return true;
}

// If the y cooodinates of a and Peach are at least yDeltaLimit apart,
// return false; otherwise, set xDeltaFromActor to the difference
// between Peach's and a's x coordinates (positive means Peach is to
// the right of a) and return true.
bool StudentWorld::getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const
{
    int aY = a->getY();
    int pY = peach->getY();
    if (abs(aY-pY) >= yDeltaLimit)
        return false;
    int differenceX = peach->getX() - a->getX();
    xDeltaFromActor = differenceX;
    return true;
}

//**********************
//overlap functions
//**********************

//generic
bool StudentWorld::isOverlap(Actor* a1, Actor* a2) const
{
    //actor 1's bounds
    int a1up = a1->getY()+SPRITE_HEIGHT-1;
    int a1down = a1->getY();
    int a1left = a1->getX();
    int a1right = a1->getX()+SPRITE_WIDTH-1;
    //actor 2's bounds
    int a2up = a2->getY()+SPRITE_HEIGHT-1;
    int a2down = a2->getY();
    int a2left = a2->getX();
    int a2right = a2->getX()+SPRITE_WIDTH-1;
    
    //comparing the two actors' bounds
    bool verticalOverlap = false;
    bool horizontalOverlap = false;
    if (a2down <= a1up && !(a2up <= a1down)) //overlap on top
        verticalOverlap = true;
    if (a2up >= a1down && !(a2down >= a1up)) //overlap on bottom
        verticalOverlap = true;
    if (a2left <= a1right && !(a2right <= a1left)) //overlap on right
        horizontalOverlap = true;
    if (a2right >= a1left && !(a2left >= a1right)) //overlap on left
        horizontalOverlap = true;
    
    return (verticalOverlap && horizontalOverlap); //need both vertical and horizontal overlap
}

//overlap with peach
bool StudentWorld::overlapsPeach(Actor* a) const
{
    return isOverlap(a, peach);
}

//if actor is overlapping peach, peach gets bonked
bool StudentWorld::bonkOverlappingPeach(Actor* bonker) const
{
    if (isOverlap(bonker, peach)) {
        peach->getBonked(false);
        return true;
    }
    return false;
}

//if actor is overlapping peach, actor gets bonked by peach
bool StudentWorld::bonkOverlappingActor(Actor* bonker) const
{
    if (isOverlap(bonker, peach)) {
        bonker->getBonked(isPeachInv());
        return true;
    }
    return false;
}

void StudentWorld::peachBonkOverlap()
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        if (overlapsPeach(*it))
            (*it)->getBonked(isPeachInv());
    }
}

//**********************
//damage functions
//**********************

// If Peach overlaps damager, damage her and return true; otherwise,
// return false.
bool StudentWorld::damageOverlappingPeach(Actor* damager) const
{
    if(isOverlap(damager, peach)) {
        peach->sufferDamageIfDamageable();
        return true;
    }
    return false;
}

// If a non-Peach actor overlaps damager, damage that non-Peach actor
// and return true; otherwise, return false.
bool StudentWorld::damageOverlappingActor(Actor* damager) const
{
    vector<Actor*>::const_iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->isDamageable() && isOverlap(*it, damager)) {
            (*it)->sufferDamageIfDamageable();
            return true;
        }
    }
    return false;
}


//**********************
//virtual functions
//**********************

//initializating game
int StudentWorld::init()
{
    //getting correct level
    Level lev(assetPath());
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << m_level << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    
    //reset everything
    m_reachedFlag = false;
    m_reachedMario = false;
    
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    if (result==Level::load_success) {
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j);
                //loading in all the actors
                switch (ge)
                {
                    case Level::block: {
                        Block* b = new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, Block::none);
                        actors.push_back(b);
                        break;
                    }
                    case Level::flower_goodie_block: {
                        Block* b = new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, Block::flower);
                        actors.push_back(b);
                        break;
                    }
                    case Level::mushroom_goodie_block: {
                        Block* b = new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, Block::mushroom);
                        actors.push_back(b);
                        break;
                    }
                    case Level::star_goodie_block: {
                        Block* b = new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, Block::star);
                        actors.push_back(b);
                        break;
                    }
                    case Level::peach: {
                         peach = new Peach(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                         break;
                    }
                    case Level::pipe: {
                        Pipe* pip = new Pipe(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(pip);
                        break;
                    }
                    case Level::flag: {
                        Flag* f = new Flag(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(f);
                        break;
                    }
                    case Level::mario: {
                        Mario* m = new Mario(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(m);
                        break;
                    }
                    case Level::piranha: {
                        Piranha* pir = new Piranha(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(pir);
                        break;
                    }
                    case Level::koopa: {
                        Koopa* k = new Koopa(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(k);
                        break;
                    }
                    case Level::goomba: {
                        Goomba* g = new Goomba(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(g);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

//making all actors move
int StudentWorld::move()
{
    //1- all actors must do something
    peach->doSomething();
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        (*it)->doSomething();
        //checking if peach is still alive
        if (!peach->isAlive()) {
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
    }
    
    //2- peach reached flag; advance to next level
    if (m_reachedFlag) {
        m_level++;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //3- peach reached mario; game over
    if (m_reachedMario) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    //4- delete dead actors
    removeDeadActors();
    
    //5- update text status
    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << " Level: " << getLevel() << " Points: " << setw(6) << getScore();
    if (peach->hasStar())
        oss << " StarPower!";
    if (peach->hasJump())
        oss << " JumpPower!";
    if (peach->hasShoot())
        oss << " ShootPower!";
    string stat = oss.str();
    setGameStatText(stat);
    
    //6- game not over, peach still alive
    return GWSTATUS_CONTINUE_GAME;
}

//destroying game
void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) {
        delete *it;
        actors.erase(it);
        it = actors.begin();
    }
    delete peach;
}

//helper functions

//removes all dead actors from game
void StudentWorld::removeDeadActors()
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete *it;
            actors.erase(it--);
        }
    }
}

//ends level
void StudentWorld::endLevel(bool isGameWon)
{
    if (isGameWon)
        m_reachedMario = true;
    else
        m_reachedFlag = true;
}
