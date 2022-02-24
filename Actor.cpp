#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"


//peach's doSomething
void Peach::doSomething()
{
    //1- check if alive, if dead return immediately
    if (!isAlive())
        return;
    
    //2- check if currently invincible
    if (m_tempInvinc) {
        m_remainingInvTicks--; //dec remaining invincible ticks
        if (m_remainingInvTicks == 0) //no more inviniciblity
            m_tempInvinc = false;
    }
    
    //3- check if currently temp invincible TODO
    
    //4- check if currently in recharge mode before firing again
    if (m_rechargeMode) {
        if (m_remainingRechargeTicks > 0) //decrease remaining recharge ticks
            m_remainingRechargeTicks--;
        if (m_remainingRechargeTicks == 0)
            m_rechargeMode = false; //can shoot again
    }
    
    //5- check if peach overlaps with another object
    //if (getWorld()->isBlockingObjectAt(getX(), getY(), this))
    //     m_hp = 1; //temp
    
    //6- check if previously jumping
    if (m_remainingJumpDis > 0) {
        int targetY = getY() + 4;
        if (!attemptToMove(getX(), targetY)) //object blocking movement
            m_remainingJumpDis = 0; //abort jump
        else
            m_remainingJumpDis--; //one step closer to reaching top of jump
    }
    //7- check if falling
    else if (!getWorld()->isBlockingObjectAt(getX(), getY(), this) && !getWorld()->isBlockingObjectAt(getX(), getY()-1, this) && !getWorld()->isBlockingObjectAt(getX(), getY()-2, this) && !getWorld()->isBlockingObjectAt(getX(), getY()-3, this))
        moveTo(getX(), getY()-4);
    
    //8- check if user pressed key
    int pressedKey;
    if (getWorld()->getKey(pressedKey)) {
        //9- if user pressed key
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
            case KEY_PRESS_UP:
            {
                if (getWorld()->isBlockingObjectAt(getX(), getY()-1, this)) {
                    if (m_jumpPower) //has jumping power
                        m_remainingJumpDis = 12;
                    else //does not have jumping power
                        m_remainingJumpDis = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            }
            case KEY_PRESS_DOWN:
            {
                if (!m_shootPower || m_remainingRechargeTicks > 0)
                    break;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                m_remainingRechargeTicks = 8;
                
                int targetX;
                if (getDirection() == 0)  //peach facing right
                    targetX = getX() + 4;
                else
                    targetX = getX() - 4;
                PeachFireball* pf = new PeachFireball(getWorld(), getDirection(), targetX, getY());
                getWorld()->addActor(pf);
            }
        }
    }
}

//returns true if successfully moved to target, false if object was blocking target
bool Peach::attemptToMove(int targetX, int targetY)
{
    if (getWorld()->isBlockingObjectAt(targetX, targetY, this)) {
        //TODO bonk!
        return false;
    }
    else {
        moveTo(targetX, targetY);
        return true;
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
