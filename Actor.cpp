#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

//**********************
//actor functions
//**********************

//if fall is possible, fall given distance
void Actor::fallIfPossible(int dist)
{
    bool fallPossible = true;
    for (int i = 0; i <= dist; i++) {
        if (!getWorld()->isMovePossible(getX(), getY()-i, this))
            fallPossible = false;
    }
    if (fallPossible)
        moveTo(getX(), getY()-dist);
}

//reverses direction
void Actor::reverseDirection()
{
    int dir = (getDirection()==0) ? 180 : 0 ;
    setDirection(dir);
}

//calculates target x and y based on direction and distance
void Actor::converDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const
{
    destx = getX();
    desty = getY();
    switch (dir)
    {
        case 0: destx += dist; break;
        case 180: destx -= dist; break;
        case 90: desty += dist; break;
        case 270: desty -= dist; break;
        default: break;
    }
}

//**********************
//all the doSomethings:
//**********************

//peach
void Peach::doSomethingAux()
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
    
    //3- check if currently temp invincible
    if (m_tempInvinc) {
        //decrement remaining invincibility ticks
        m_remainingInvTicks--;
        if (m_remainingInvTicks==0) //no longer invincible; invincibility ran out
            m_tempInvinc = false;
    }
    
    //4- check if currently in recharge mode before firing again
    if (m_rechargeMode) {
        if (m_remainingRechargeTicks > 0) //decrease remaining recharge ticks
            m_remainingRechargeTicks--;
        if (m_remainingRechargeTicks == 0)
            m_rechargeMode = false; //can shoot again
    }
    
    //5- check if peach overlaps with another object
    getWorld()->bonkOverlappingPeach(this);
    
    //6- check if previously jumping
    if (m_remainingJumpDis > 0) {
        int targetY = getY() + 4;
        if (!getWorld()->moveOrBonk(this, getX(), targetY)) //object blocking movement
            m_remainingJumpDis = 0; //abort jump
        else {
            moveTo(getX(), targetY); //move up four pixels
            m_remainingJumpDis--; //one step closer to reaching top of jump
        }
    }
    //7- check if falling
    else
        fallIfPossible(4);
    
    //8- check if user pressed key
    int pressedKey;
    if (getWorld()->getKey(pressedKey)) {
        //9- if user pressed key
        switch(pressedKey) {
            case KEY_PRESS_LEFT:
            {
                setDirection(180);
                int targetX = getX()-4;
                getWorld()->moveOrBonk(this, targetX, getY());
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setDirection(0);
                int targetX = getX()+4;
                getWorld()->moveOrBonk(this, targetX, getY());
                break;
            }
            case KEY_PRESS_UP:
            {
                if (!getWorld()->isMovePossible(getX(), getY()-1, this)) {
                    if (m_jumpPower) //has jumping power
                        m_remainingJumpDis = 12;
                    else //does not have jumping power
                        m_remainingJumpDis = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if (!m_shootPower || m_remainingRechargeTicks > 0)
                    return;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                m_remainingRechargeTicks = 8;
                m_rechargeMode = true;
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

//enemy (for goomba and koopa)
void Enemy::doSomethingAux()
{
    //2- check if overlaps w peach
    if (getWorld()->bonkOverlappingPeach(this))
        return;
    
    //3&4- cannot move in current direction
    switch (getDirection())
    {
        case 180: { //left
            if (!getWorld()->isMovePossible(getX()-1, getY(), this))
                setDirection(0);
            if (getWorld()->isMovePossible(getX()-1-SPRITE_WIDTH, getY()-1, this))
                setDirection(0);
            break;
        }
        case 0: {
            if (!getWorld()->isMovePossible(getX()+1, getY(), this))
                setDirection(180);
            if (getWorld()->isMovePossible(getX()+1+SPRITE_WIDTH, getY()-1, this))
                setDirection(180);
            break;
        }
    }
    
    //5- move if possible
    int targetX = 0;;
    switch (getDirection())
    {
        case 0: targetX = getX()+1; //right
            break;
        case 180: targetX = getX()-1; //left
            break;
    }
    getWorld()->moveIfPossible(this, targetX, getY());
}

//piranha
void Piranha::doSomethingAux()
{
    //2- cycle sprite image
    increaseAnimationNumber();
    
    //3- check if overlaps w peach
    if (getWorld()->bonkOverlappingPeach(this))
        return;
    
    //4- determining if peach is within range
    int xDifference;
    if (!getWorld()->getPeachTargetingInfo(this, 1.5*SPRITE_HEIGHT, xDifference))
        return;
    //checking direction
    int dir = (xDifference > 0) ? 0 : 180;
    setDirection(dir);
    
    //check firing delay
    if (m_fireDelay > 0) {
        m_fireDelay--;
        return;
    }
    
    //peach is within range x
    if (abs(xDifference) < 8*SPRITE_WIDTH) {
        PirFireball* fireball = new PirFireball(getWorld(), getDirection(), getX(), getY());
        getWorld()->addActor(fireball);
        getWorld()->playSound(SOUND_PIRANHA_FIRE);
        m_fireDelay = 40;
    }
}

//goodies
//powerup general
void Powerup::doSomethingAux()
{
    if (getWorld()->overlapsPeach(this)) {
        grantGoodie();
        setAlive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    //move 2 pixels down if possible
    if (!getWorld()->moveIfPossible(this, getX(), getY()-2)) {
        //if not possible do this-
        int targetX = (getDirection()==0) ? getX()+2 : getX()-2;
        if (!getWorld()->moveIfPossible(this, targetX, getY())) {
            reverseDirection();
            return;
        }
    }
}

//flower
void Flower::grantGoodie()
{
    getWorld()->increaseScore(50);
    getWorld()->grantShootPower();
    getWorld()->setPeachHP(2);
}

//star
void Star::grantGoodie()
{
    getWorld()->increaseScore(100);
    getWorld()->grantInvincibility(150);
}

//mushroom
void Mushroom::grantGoodie()
{
    getWorld()->increaseScore(75);
    getWorld()->grantJumpPower();
    getWorld()->setPeachHP(2);
}

//projectile general
void Projectile::doSomethingAux()
{
    //1- overlap w damageable actor
    if (getWorld()->damageOverlappingActor(this)) {
        setAlive(false);
        return;
    }
    
    //2&3-
    projectileMove();
}

//piranha fireball
void PirFireball::doSomethingAux()
{
    //1- overlap w peach
    if (getWorld()->damageOverlappingPeach(this)) {
        setAlive(false);
        return;
    }
        
    //2&3-
    projectileMove();
}


//**********************
//getBonked functions
//**********************

//block
void Block::getBonked(bool bonkerIsInvinciblePeach)
{
    if (m_goodieReleased || m_goodie == none)
        getWorld()->playSound(SOUND_PLAYER_BONK);
    else {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        switch (m_goodie)
        {
            case flower: {
                Flower* fl = new Flower(getWorld(), getX(), getY()+8);
                getWorld()->addActor(fl);
                m_goodieReleased = true;
                break;
            }
            case star: {
                Star* st = new Star(getWorld(), getX(), getY()+8);
                getWorld()->addActor(st);
                m_goodieReleased = true;
                break;
            }
            case mushroom: {
                Mushroom* mu = new Mushroom(getWorld(), getX(), getY()+8);
                getWorld()->addActor(mu);
                m_goodieReleased = true;
                break;
            }
            case none: {
                break;
            }
        }
    }
}

//enemies
void Enemy::getBonked(bool bonkerIsInvinciblePeach)
{
    if (bonkerIsInvinciblePeach) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        die();
    }
}

//koopa
void Koopa::getBonked(bool bonkerIsInvinciblePeach)
{
    if (bonkerIsInvinciblePeach) {
        Enemy::getBonked(true);
        introduceShell();
    }
}

//**********************
//helper functions
//**********************

//the generic projectile move function
void Projectile::projectileMove()
{
    //2- fall 2 pixels down if possible
    fallIfPossible(2);
    //attempt to move
    int destx, desty;
    converDirectionAndDistanceToXY(getDirection(), 2, destx, desty);
    if (!getWorld()->moveIfPossible(this, destx, desty)) {
        setAlive(false);
        return;
    }
}

//koopa die!
void Enemy::die()
{
    getWorld()->increaseScore(100);
    setAlive(false);
}

//creates new shell object
void Koopa::introduceShell()
{
    Shell* s = new Shell(getWorld(), getDirection(), getX(), getY());
    getWorld()->addActor(s);
}

//kills koopa object and introduces shell
void Koopa::sufferDamageIfDamageable()
{
    die();
    introduceShell();
}

