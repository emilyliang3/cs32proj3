#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

bool StudentWorld::isBlockingObjectAt(int x, int y, Actor* a)
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        Actor* a2 = *it;
        if (a2->isSolid()) {
            if (x+SPRITE_WIDTH-1 >= a2->getX() && x <= a2->getX()+SPRITE_WIDTH-1 && y+SPRITE_WIDTH-1 >= a2->getY() && y <= a2->getY()+SPRITE_HEIGHT-1)
                return true;
        }
    }
    return false;
}

bool StudentWorld::isOverlap(Actor* a1, Actor* a2)
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
    if (a2down <= a1up && !(a2up <= a1down)) //overlap on top
        return true;
    if (a2up >= a1down && !(a2down >= a1up)) //overlap on bottom
        return true;
    if (a2left <= a1right && !(a2right <= a1left)) //overlap on right
        return true;
    if (a2right >= a1left && !(a2left >= a1right)) //overlap on left
        return true;
    
    return false;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    
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
                        Block* b = new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(b);
                        break; }
                    case Level::peach: {
                        Peach* peach = new Peach(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(peach);
                        break; }
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

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        (*it)->doSomething();
    }
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); )
        it = actors.erase(it);
}
