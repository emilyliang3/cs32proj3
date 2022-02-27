#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//actor
class Actor : public GraphObject
{
public:
    //constructor
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth, double size) : GraphObject(imageID, startX, startY, startDir, depth, size), m_world(world), m_alive(true), m_solid(false) {};
    
    //virtual
    virtual void doSomething() { if (isAlive()) doSomethingAux(); } ;
    
    //getters
    bool isAlive() { return m_alive; }
    bool isSolid() { return m_solid; }
    StudentWorld* getWorld() const { return m_world; }
    virtual bool isDamageable() { return false; }
    
    //setters
    void setAlive(bool b) { m_alive = b; }
    void setSolid(bool b) { m_solid = b; }
    
    // Bonk this actor.  Parameter says whether bonker is Peach with invincibiity.
    virtual void getBonked(bool bonkerIsInvinciblePeach) {} ;
    // Do what the spec says happens when damage is inflicted on this actor.
    virtual void sufferDamageIfDamageable() {} ;

    void fallIfPossible(int dist);
    void reverseDirection();

    // Set destx and desty to the coordinates dist pixels away in direction
    // dir from this actor's position.
    void converDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const;
    
private:
    virtual void doSomethingAux() = 0;
    StudentWorld* m_world;
    bool m_alive;
    bool m_solid;
};


//enemies (piranha, goomba, koopa)
class Enemy : public Actor
{
public:
    Enemy(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, randInt(0,1)*180, 0, 1) {};
    virtual void getBonked(bool bonkerIsInvinciblePeach);
    virtual bool isDamageable() { return true; }
    virtual void sufferDamageIfDamageable() { die(); }
    void die();
private:
    virtual void doSomethingAux();
};

//piranha
class Piranha : public Enemy
{
public:
    Piranha(StudentWorld* world, int startX, int startY) : Enemy (world, IID_PIRANHA, startX, startY), m_fireDelay(0) {} ;
private:
    virtual void doSomethingAux(); //different than the generic enemy doSomething
    int m_fireDelay;
};

//goomba
class Goomba : public Enemy
{
public:
    Goomba(StudentWorld* world, int startX, int startY) : Enemy (world, IID_GOOMBA, startX, startY) {} ;
private:
};

//koopa
class Koopa : public Enemy
{
public:
    Koopa(StudentWorld* world, int startX, int startY) : Enemy (world, IID_KOOPA, startX, startY) {} ;
    virtual void getBonked(bool bonkerIsInvinciblePeach); //different from generic enemy
    virtual void sufferDamageIfDamageable(); //different from generic enemy
private:
    void introduceShell(); //when koopa dies; creates new shell object at place of death
};

//peach
class Peach : public Actor
{
public:
    //constructor
    Peach(StudentWorld* world, int startX, int startY) : Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0) {
        m_hp = 1;
        m_tempInvinc = false;
        m_starPower = false;
        m_shootPower = false;
        m_jumpPower = false;
        m_remainingInvTicks = 0;
        m_remainingJumpDis = 0;
        m_remainingRechargeTicks = 0;
        m_rechargeMode = false;
    };
    
    //setters
    void setShoot() { m_shootPower = true; }
    void setStar() { m_starPower = true; }
    void setHP(int hp) { m_hp = hp; }
    void setJump() { m_jumpPower = true; }
    void setInvinc() { m_tempInvinc = true; }
    void grantInvinc(int ticks) { m_remainingInvTicks = ticks; }
    
    //accessors
    bool getInvinc() { return m_tempInvinc; }
    bool hasStar() { return m_starPower; }
    bool hasShoot() { return m_shootPower; }
    bool hasJump() { return m_jumpPower; }
    
    //other
    virtual bool isDamageable() { return true; }
    virtual void getBonked(bool bonkerIsInvinciblePeach);
    virtual void sufferDamageIfDamageable() { getBonked(false); }
private:
    int m_hp;
    
    bool m_tempInvinc;
    int m_remainingInvTicks;
    
    bool m_starPower;
    
    bool m_shootPower;
    bool m_rechargeMode;
    int m_remainingRechargeTicks;
    
    bool m_jumpPower;
    int m_remainingJumpDis;
    
    virtual void doSomethingAux();
};


//powerups (flowers, mushroom, star)
class Powerup : public Actor
{
public:
    Powerup(StudentWorld* world, int imageID, int startX, int startY) : Actor (world, imageID, startX, startY, 0, 1, 1) {} ;
    virtual void grantGoodie() {} ;
private:
    virtual void doSomethingAux();
};

//star
class Star : public Powerup
{
public:
    Star(StudentWorld* world, int startX, int startY) : Powerup (world, IID_STAR, startX, startY) {} ;
    virtual void grantGoodie();
private:
};

//mushroom
class Mushroom : public Powerup
{
public:
    Mushroom(StudentWorld* world, int startX, int startY) : Powerup (world, IID_MUSHROOM, startX, startY) {} ;
    virtual void grantGoodie();
private:
};

//flower
class Flower : public Powerup
{
public:
    Flower(StudentWorld* world, int startX, int startY) : Powerup (world, IID_FLOWER, startX, startY) {} ;
    virtual void grantGoodie();
private:
};


//projectiles (fireballs, shell)
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* world, int imageID, int dir, int startX, int startY) : Actor (world, imageID, startX, startY, dir, 1, 1) {} ;
    void projectileMove();
private:
    virtual void doSomethingAux();
};

//piranha fireball
class PirFireball : public Projectile
{
public:
    PirFireball(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_PIRANHA_FIRE, dir, startX, startY) {} ;
private:
    virtual void doSomethingAux();
};

//peach fireball
class PeachFireball : public Projectile
{
public:
    PeachFireball(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_PEACH_FIRE, dir, startX, startY) {} ;
private:
};

//shell
class Shell : public Projectile
{
public:
    Shell(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_SHELL, dir, startX, startY) {} ;
private:
};


//things that don't move (block, pipe)
class Stationary : public Actor
{
public:
    Stationary(StudentWorld* world, int imageID, int startX, int startY) : Actor (world, imageID, startX, startY, 0 , 2, 1) { setSolid(true); };
private:
    virtual void doSomethingAux() {} ;
};

//block
class Block : public Stationary
{
public:
    enum blockType { none, flower, star, mushroom };
    Block(StudentWorld* world, int startX, int startY, blockType bt) : Stationary(world, IID_BLOCK, startX, startY), m_goodie(bt), m_goodieReleased(false) {};
    virtual void getBonked(bool bonkerIsInvinciblePeach);
private:
    blockType m_goodie;
    bool m_goodieReleased;
    
};

//pipe
class Pipe : public Stationary
{
public:
    Pipe(StudentWorld* world, int startX, int startY) : Stationary(world, IID_PIPE, startX, startY) {};
};


//things that transport to next level (flag, mario)
class Portal : public Actor
{
public:
    Portal(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, 0, 1, 1) {} ;
private:
    virtual void doSomethingAux();
    virtual void levelComplete() = 0;
};

//flag
class Flag : public Portal
{
public:
    Flag(StudentWorld* world, int startX, int startY) : Portal(world, IID_FLAG, startX, startY) {} ;
private:
    virtual void levelComplete();
};

//mario
class Mario : public Portal
{
public:
    Mario(StudentWorld* world, int startX, int startY) : Portal(world, IID_MARIO, startX, startY) {} ;
private:
    virtual void levelComplete();
};





#endif // ACTOR_H_
