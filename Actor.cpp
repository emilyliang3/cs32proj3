#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Peach::doSomething()
{
    int pressedKey;
    if (getWorld()->getKey(pressedKey)) {
        switch(pressedKey) {
            case KEY_PRESS_LEFT:
            {
                setDirection(180);
                int targetX = getX()-4;
                attemptToMove(targetX, getY());
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setDirection(0);
                int targetX = getX()+4;
                attemptToMove(targetX, getY());
                break;
            }
        }
    }
}

void Peach::attemptToMove(int targetX, int targetY)
{
    if (getWorld()->isBlockingObjectAt(targetX, targetY, this)) {
        //bonk!
    }
    else {
        moveTo(targetX, targetY);
    }
}

//returns 0 or 180, each with a 50% chance
int Enemy::getRandDir()
{
    int ran = rand() % 2;
    if (ran == 1)
        return 180;
    return 0;

}
