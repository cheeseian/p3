#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int dir, double size);
    virtual void doSomething();
    virtual int canWalkThrough();   // 1 PickupableItem, 2 Marble, 0 everything else
    virtual int canMarbleThrough(); // 1 for Pit, 0 for everything else
    virtual int isCollideable();    // 2 for player/robot, 1 for wall/factory, 0 for everything else
    virtual bool canShootThrough(); // true for Pickupable itme, 0 everthing else
    virtual int ammoAmount(); // defaulted 0 for everyone but player
    virtual int getHealth();
    virtual void addHealth();
    virtual void addPeas(int num);
    virtual bool canControl(); // to identify player
    virtual void recieveDamage();
    bool isDead();
    void setDead();

private:
    bool m_isDead;
};

class Avatar : public Actor
{
public:
    Avatar(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
    virtual void doSomething();
    virtual int canWalkThrough();
    virtual bool canControl();
    virtual int isCollideable();
    virtual bool canShootThrough();
    virtual void recieveDamage();
    virtual int ammoAmount();
    virtual int getHealth();
    virtual void addHealth();
    virtual void addPeas(int num);

private:
    StudentWorld *m_studentWorld;
    int m_numPeas;
    int m_hitPoints;
};

class Robot : public Actor
{
public:
    Robot(int imageID, double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual int canWalkThrough();
    virtual int isCollideable();
    virtual void recieveDamage();
    bool isActiveTick();
private:
    StudentWorld* m_studentWorld;
    int m_hitPoints;
    int m_ticks;
    int m_tickCount;
};

class RageBot : public Robot
{
public:
    RageBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
    virtual void doSomething();
private:
    StudentWorld* m_studentWorld;
};

class ThiefBot : public Robot
{
public:
    ThiefBot(int imageID, double startX, double startY, int dir, double size, StudentWorld *studentWorld);
};

class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
};

class Wall : public Actor
{
public:
    Wall(double startX, double startY, int dir, double size);
    virtual int canWalkThrough();
    virtual int isCollideable();
};

class Marble : public Actor
{
public:
    Marble(double startX, double startY, int dir, double size);
    virtual int canWalkThrough();
    virtual int isCollideable();
    virtual void recieveDamage();

private:
    int m_hitPoints;
};

class Pit : public Actor
{
public:
    Pit(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
    virtual int canWalkThrough();
    virtual int canMarbleThrough();
    virtual bool canShootThrough();
    virtual void doSomething();

private:
    StudentWorld *m_studentWorld;
};

class Exit : public Actor
{
public:
    Exit(double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual void doSomething();
private:
    StudentWorld* m_studentWorld;
};

class Pea : public Actor
{
public:
    Pea(double startX, double startY, int dir, double size, StudentWorld *studentWorld);
    void doSomething();

private:
    StudentWorld *m_studentWorld;
};

class ThiefBotFactory : public Actor
{
public:
    ThiefBotFactory(double startX, double startY, int dir, double size, bool isMean);
    virtual int canWalkThrough();
    virtual int isCollideable();

private:
    bool m_isMean;
};

class PickupableItem : public Actor
{
public:
    PickupableItem(int imageID, double startX, double startY, int dir, double size);
    virtual bool canShootThrough();
};

class Crystal : public PickupableItem
{
public:
    Crystal(double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual void doSomething();
private:
    StudentWorld* m_studentWorld;
};

class Goodie : public PickupableItem
{
public:
    Goodie(int imageID, double startX, double startY, int dir, double size, StudentWorld* studentWorld, int scoreAmount);
    virtual void doSomething();
    virtual void updatePlayerStat() = 0;
private:
    StudentWorld* m_studentWorld;
    int m_scoreAmount;
};

class ExtraLife : public Goodie
{
public:
    ExtraLife(double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual void updatePlayerStat();
private:
    StudentWorld* m_studentWorld;
};

class RestoreHealth : public Goodie
{
public:
    RestoreHealth(double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual void updatePlayerStat();
private:
    StudentWorld* m_studentWorld;
};

class Ammo : public Goodie
{
public:
    Ammo(double startX, double startY, int dir, double size, StudentWorld* studentWorld);
    virtual void updatePlayerStat();
private:
    StudentWorld* m_studentWorld;
};

#endif // ACTOR_H_
