#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//helper functions
int getRandomDir()
{
    int ran = rand() % 2;
    if (ran == 1)
        return 180;
    return 0;
}


//actor
class Actor : public GraphObject
{
public:
    //constructor
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth, double size) : GraphObject(imageID, startX, startY, startDir, depth, size), m_world(world), m_alive(true), m_solid(false) {};
    
    //pure virtual
    virtual void doSomething() = 0;
    
    //getters
    bool isAlive() { return m_alive; }
    bool isSolid() { return m_solid; }
    StudentWorld* getWorld() const { return m_world; }
    
    //setters
    void setAlive(bool b) { m_alive = b; }
    void setSolid(bool b) { m_solid = b; }
    
    
private:
    StudentWorld* m_world;
    bool m_alive;
    bool m_solid;
};


//enemies (piranha, goomba, koopa)
class Enemy : public Actor
{
public:
    Enemy(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth, double size) : Actor(world, imageID, startX, startY, getRandomDir(), 0, 1) {};
    virtual void doSomething() {} ;
};

//piranha
class Piranha : public Enemy
{
public:
    Piranha(StudentWorld* world, int startX, int startY) : Enemy (world, IID_PIRANHA, startX, startY) {} ;
    virtual void doSomething() {} ;
}

//goomba
class Goomba : public Enemy
{
public:
    Goomba(StudentWorld* world, int startX, int startY) : Enemy (world, IID_GOOMBA, startX, startY) {} ;
    virtual void doSomething() {} ;
}


//peach
class Peach : public Actor
{
public:
    //constructor
    Peach(StudentWorld* world, int startX, int startY) : Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0) {
        m_hp = 1;
        m_tempInvisibility = false;
        m_starPower = false;
        m_shootPower = false;
        m_jumpPower = false;
    };
    
    //other
    virtual void doSomething();
    void attemptToMove(int targetX, int targetY);
private:
    int m_hp;
    bool m_tempInvisibility;
    bool m_starPower;
    bool m_shootPower;
    bool m_jumpPower;
};


//powerups (flowers, mushroom, star)
class Powerup : public Actor
{
public:
    Powerup(StudentWorld* world, int imageID, int startX, int startY) : Actor (world, imageID, startX, startY, 0, 1, 1) {} ;
    virtual void doSomething() {} ;
};

//star
class Star : public Powerup
{
public:
    Star(StudentWorld* world, int startX, int startY) : Powerup (world, IID_STAR, startX, startY) {} ;
    virtual void doSomething() {} ;
}

//mushroom
class Mushroom : public Powerup
{
public:
    Mushroom(StudentWorld* world, int startX, int startY) : Powerup (world, IID_MUSHROOM, startX, startY) {} ;
    virtual void doSomething() {} ;
}

//flower
class Flower : public Powerup
{
public:
    Flower(StudentWorld* world, int startX, int startY) : Powerup (world, IID_FLOWER, startX, startY) {} ;
    virtual void doSomething();
}


//projectiles (fireballs, shell)
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* world, int imageID, int dir, int startX, int startY) : Actor (world, imageID, startX, startY, dir, 1, 1) {} ;
    virtual void doSomething() {} ;
};

//piranha fireball
class PirFireball : public Projectile
{
public:
    PirFireball(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_PIRANHA_FIRE, dir, startX, startY) {} ;
    virtual void doSomething() {} ;
}

//peach fireball
class PeachFireball : public Projectile
{
public:
    PeachFireball(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_PEACH_FIRE, dir, startX, startY) {} ;
    virtual void doSomething() {} ;
}

//shell
class Shell : public Projectile
{
public:
    Shell(StudentWorld* world, int dir, int startX, int startY) : Projectile (world, IID_SHELL, dir, startX, startY) {} ;
    virtual void doSomething() {};
}


//things that don't move (block, pipe)
class Stationary : public Actor
{
public:
    Stationary(StudentWorld* world, int imageID, int startX, int startY) : Actor (world, imageID, startX, startY, 0 , 2, 1) { setSolid(true); };
    virtual void doSomething() {} ;
private:
    
};

//block
class Block : public Stationary
{
public:
    Block(StudentWorld* world, int startX, int startY) : Stationary(world, IID_BLOCK, startX, startY) {};
    virtual void doSomething() {} ;
};

//pipe
class Pipe : public Stationary
{
public:
    Pipe(StudentWorld* world, int startX, int startY) : Stationary(world, IID_PIPE, startX, startY) {};
    virtual void doSomething() {} ;
};


//things that transport to next level (flag, mario)
class Portal : public Actor
{
public:
    Portal(StudentWorld* world, int imageID, int startX, int startY) : Stationary(world, imageID, startX, startY, 0, 1, 1) {} ;
    virtual void doSomething() {} ;
}

//flag
class Flag : public Portal
{
public:
    Flag(StudentWorld* world, int startX, int startY) : Portal(world, IID_FLAG, startx, startY) {} ;
    virtual void doSomething() {};
}

//mario
class Mario : public Portal
{
public:
    Mario(StudentWorld* world, int startX, int startY) : Portal(world, IID_MARIO, startX, startY) {} ;
    virtual void doSomething() {};
}





#endif // ACTOR_H_
