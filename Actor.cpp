#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor implementation
Actor::Actor(int imageID, double startX, double startY, int dir, double size)
    : GraphObject(imageID, startX, startY, dir, size), m_isDead(false)
{
}
void Actor::doSomething()
{
}
bool Actor::isDead()
{
    return m_isDead;
}
void Actor::setDead()
{
    m_isDead = true;
}
int Actor::canWalkThrough()
{
    return 1; // default we can walk through
}
int Actor::canMarbleThrough()
{
    return 0; // default can't push marbles through actors
}
int Actor::isCollideable()
{
    return 0; // default peas go through
}
bool Actor::canShootThrough()
{
    return false; // default can't shoot through
}
int Actor::ammoAmount()
{
    return 0; // default doesn't have ammo
}
int Actor::getHealth()
{
    return 0; // default has no health
}
void Actor::addPeas(int num)
{
    return;
}
void Actor::addHealth()
{
    return;
}
bool Actor::canControl()
{
    return false; // can't control most actors
}
bool Actor::canBeMunched()
{
    return false;
}
bool Actor::canMunch()
{
    return false;
}
void Actor::recieveDamage()
{
    return; // default deal no damage
}

// Player implementation
Avatar::Avatar(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Actor(IID_PLAYER, startX, startY, dir, size), m_studentWorld(studentWorld), m_numPeas(20), m_hitPoints(20)
{
    setVisible(true);
}
int Avatar::canWalkThrough()
{
    return 0; // robots can't walk thorugh
}
bool Avatar::canControl()
{
    return true;
}
int Avatar::isCollideable()
{
    return 2; // can be damaged
}
bool Avatar::canShootThrough()
{
    return true;
}
void Avatar::recieveDamage()
{
    m_studentWorld->playSound(SOUND_PLAYER_IMPACT);
    m_hitPoints -= 2;
    if (m_hitPoints <= 0)
    {
        m_studentWorld->playSound(SOUND_PLAYER_DIE);
        setDead();
    }
}
int Avatar::ammoAmount()
{
    return m_numPeas;
}
int Avatar::getHealth()
{
    return m_hitPoints;
}
void Avatar::addHealth()
{ 
    m_hitPoints = 20;

}
void Avatar::addPeas(int num)
{
    m_numPeas += num;
    cerr << "peas aded" << m_numPeas << endl;
}

void Avatar::doSomething()
{
    if (isDead())
        return;
    // if a key is pressed, move in corresponding direction
    int ch;
    if (m_studentWorld->getKey(ch))
    {

        switch (ch)
        {
        case KEY_PRESS_LEFT:
            setDirection(left);
            if (m_studentWorld->canMoveAt(getX() - 1, getY(), left))
            {
                moveTo(getX() - 1, getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            setDirection(right);
            if (m_studentWorld->canMoveAt(getX() + 1, getY(), right))
            {
                moveTo(getX() + 1, getY());
            }
            break;
        case KEY_PRESS_UP:
            setDirection(up);
            if (m_studentWorld->canMoveAt(getX(), getY() + 1, up))
            {
                moveTo(getX(), getY() + 1);
            }
            break;
        case KEY_PRESS_DOWN:
            setDirection(down);
            if (m_studentWorld->canMoveAt(getX(), getY() - 1, down))
            {
                moveTo(getX(), getY() - 1);
            }
            break;
        case KEY_PRESS_SPACE:
            // create a pea in direction
            if (!(ammoAmount() <= 0))
            {
                m_studentWorld->shootPea(getX(), getY(), getDirection());
                m_studentWorld->playSound(SOUND_PLAYER_FIRE);
                addPeas(-1);
            }
            break;
        case KEY_PRESS_ESCAPE:
            m_studentWorld->escapePressed();
        }
    }
}

// Robot implementation
Robot::Robot(int imageID, double startX, double startY, int dir, double size, StudentWorld* studentWorld)
    : Actor(imageID, startX, startY, dir, size), m_studentWorld(studentWorld), m_hitPoints(10), m_tickCount(0)
{
    m_ticks = (28 - m_studentWorld->getLevel()) / 4;
    if (m_ticks < 3)
        m_ticks = 3;
}
int Robot::canWalkThrough()
{
    return 0; // Can't walk through!
}
int Robot::isCollideable()
{
    return 2; // can be damaged
}
void Robot::recieveDamage()
{
    m_studentWorld->playSound(SOUND_ROBOT_IMPACT);
    m_hitPoints -= 2;
    if (m_hitPoints <= 0)
    {
        m_studentWorld->playSound(SOUND_ROBOT_DIE);
        setDead();
    }
}
bool Robot::isActiveTick()
{
    if (m_tickCount >= m_ticks)
    {
        m_tickCount = 0;
        return true;
    } else {
        m_tickCount++;
        return false;
    }
}

// RageBot implementation
RageBot::RageBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Robot(IID_RAGEBOT, startX, startY, dir, size, studentWorld), m_studentWorld(studentWorld)
{
}
void RageBot::doSomething()
{
    if (isDead())
        return;
    if (isActiveTick())
    {
        // check if player is in vision, if so shoot a pea
        if (m_studentWorld->playerInView(getX(), getY(), getDirection()))
        {
            m_studentWorld->playSound(SOUND_ENEMY_FIRE);
            m_studentWorld->shootPea(getX(), getY(), getDirection());
            return;
        }
        // check if can move, if so do so. Else, flip direction
        int xoff, yoff;
        m_studentWorld->getOffsets(xoff, yoff, getDirection());
        if (m_studentWorld->robotCanMoveAt(getX() + xoff, getY() + yoff, getDirection()))
        {
            moveTo(getX() + xoff, getY() + yoff);
        } else {
            switch(getDirection())
            {
                case 0:
                    setDirection(180);
                    break;
                case 90:
                    setDirection(270);
                    break;
                case 180:
                    setDirection(0);
                    break;
                case 270:
                    setDirection(90);
                    break;
                default:
                    break;
            }
        }
    }
}

// ThiefBot implementation
ThiefBot::ThiefBot(int imageID, double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Robot(imageID, startX, startY, dir, size, studentWorld), m_studentWorld(studentWorld), m_carryingGoodie(false)
{
    distanceBeforeTurning = randInt(1, 6);
    setVisible(true);
}
bool ThiefBot::canMunch()
{
    return true;
}
void ThiefBot::doSomething()
{
    if (isDead())
        return;
    if (isActiveTick())
    {
        if (!m_carryingGoodie && m_studentWorld->tryToMunch(getX(), getY()))
        {
            m_carryingGoodie = true;
            return; // ends action if successful munch happens
        }

        
    }
}

// RegularThiefBot implementation
RegularThiefBot::RegularThiefBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : ThiefBot(IID_THIEFBOT, startX, startY, dir, size, studentWorld), m_hitPoints(5)
{
}

// MeanThiefBot implementation
MeanThiefBot::MeanThiefBot(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : ThiefBot(IID_MEAN_THIEFBOT, startX, startY, dir, size, studentWorld), m_hitPoints(8)
{
}

// Wall implementation
Wall::Wall(double startX, double startY, int dir, double size)
    : Actor(IID_WALL, startX, startY, dir, size)
{
    setVisible(true);
}
int Wall::canWalkThrough()
{
    return 0; // Can't walk through!
}
int Wall::isCollideable()
{
    return 1; // can be collided, but not damaged
}

// Marble implementation
Marble::Marble(double startX, double startY, int dir, double size)
    : Actor(IID_MARBLE, startX, startY, dir, size), m_hitPoints(10)
{
    setVisible(true);
}
int Marble::canWalkThrough()
{
    return 2; // Special case!
}
int Marble::isCollideable()
{
    return 2; // can be damaged!
}
void Marble::recieveDamage()
{
    m_hitPoints -= 2;
    if (m_hitPoints <= 0)
    {
        setDead();
    }
}

// Pit implementation
Pit::Pit(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Actor(IID_PIT, startX, startY, dir, size), m_studentWorld(studentWorld)
{
    setVisible(true);
}
int Pit::canWalkThrough()
{
    return 0; // Can't walk through!
}
int Pit::canMarbleThrough()
{
    return 1;
}
bool Pit::canShootThrough()
{
    return true; // can shoot over pits;
}
void Pit::doSomething()
{
    if (m_studentWorld->checkIfMarbleAt(getX(), getY()))
    {
        setDead();
    }
}

// Exit implementation
Exit::Exit(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Actor(IID_EXIT, startX, startY, dir, size), m_studentWorld(studentWorld), m_isVisible(false)
{
    setVisible(false);
}
void Exit::doSomething()
{
    if (m_studentWorld->crystalsLeft() <= 0)
    {
        m_studentWorld->playSound(SOUND_REVEAL_EXIT);
        m_isVisible = true;
        setVisible(true);
    }
    if (m_isVisible && m_studentWorld->checkIfPlayerAt(getX(), getY()))
    {
        m_studentWorld->playSound(SOUND_FINISHED_LEVEL);
        m_studentWorld->levelComplete();
    }

}

// Pea implementation
Pea::Pea(double startX, double startY, int dir, double size, StudentWorld *studentWorld)
    : Actor(IID_PEA, startX, startY, dir, size), m_studentWorld(studentWorld)
{
    setVisible(true);
}
void Pea::doSomething()
{
    if (isDead())
    {
        return;
    }

    if (m_studentWorld->checkPeaSquare(getX(), getY()))
    {
        setDead();
        return;
    }

    // Move in specified direction
    int xOffset, yOffset;
    m_studentWorld->getOffsets(xOffset, yOffset, getDirection());
    moveTo(getX() + xOffset, getY() + yOffset);
}

// Thief Bot Factory implementation
ThiefBotFactory::ThiefBotFactory(double startX, double startY, int dir, double size, bool isMean, StudentWorld* studentWorld)
    : Actor(IID_ROBOT_FACTORY, startX, startY, dir, size), m_isMean(isMean), m_studentWorld(studentWorld)
{
    setVisible(true);
}
int ThiefBotFactory::canWalkThrough()
{
    return 0; // Can't walk through!
}
int ThiefBotFactory::isCollideable()
{
    return 1; // is collideable, not damageable.
}
void ThiefBotFactory::doSomething()
{
    if(m_studentWorld->countThiefBots(getX(), getY()) < 3)
    {
        if(randInt(1, 50) == 1)
        {
            m_studentWorld->spawnThiefBot(getX(), getY(), m_isMean);
        }
    }
}
// Pickupable Item
PickupableItem::PickupableItem(int imageID, double startX, double startY, int dir, double size)
    : Actor(imageID, startX, startY, dir, size)
{
    setVisible(true);
}
bool PickupableItem::canShootThrough()
{
    return true;
}

// Crystal implementation
Crystal::Crystal(double startX, double startY, int dir, double size, StudentWorld* studentWorld)
    : PickupableItem(IID_CRYSTAL, startX, startY, dir, size), m_studentWorld(studentWorld)
{
}
void Crystal::doSomething()
{
    if(isDead())
        return;
    if(m_studentWorld->checkIfPlayerAt(getX(), getY()))
        {
            m_studentWorld->playSound(SOUND_GOT_GOODIE);
            m_studentWorld->subtractCrystal();
            m_studentWorld->increaseScore(50);
            setDead();
            // play sound effect
        }
}

// Goodie implementation
Goodie::Goodie(int imageID, double startX, double startY, int dir, double size, StudentWorld* studentWorld, int scoreAmount)
    : PickupableItem(imageID, startX, startY, dir, size), m_studentWorld(studentWorld), m_scoreAmount(scoreAmount), m_isVisible(true)
{
}
bool Goodie::canBeMunched()
{
    return true;
}
void Goodie::doSomething()
{
    if (isDead())
        return;
    if (m_studentWorld->checkIfPlayerAt(getX(), getY()) && m_isVisible)
    {
        m_studentWorld->playSound(SOUND_GOT_GOODIE);
        m_studentWorld->increaseScore(m_scoreAmount);
        updatePlayerStat();
        setDead();
        // play sound effect
    }
}

// Extra life goodie implementation
ExtraLife::ExtraLife(double startX, double startY, int dir, double size, StudentWorld* studentWorld)
    : Goodie(IID_EXTRA_LIFE, startX, startY, dir, size, studentWorld, 1000), m_studentWorld(studentWorld)
{
}
void ExtraLife::updatePlayerStat()
{
    m_studentWorld->incLives();
}

// Restore Health implementation
RestoreHealth::RestoreHealth(double startX, double startY, int dir, double size, StudentWorld* studentWorld)
    : Goodie(IID_RESTORE_HEALTH, startX, startY, dir, size, studentWorld, 500), m_studentWorld(studentWorld)
{
}
void RestoreHealth::updatePlayerStat()
{
    m_studentWorld->restoreHealth();
}
// Ammo implementation
Ammo::Ammo(double startX, double startY, int dir, double size, StudentWorld* studentWorld)
    : Goodie(IID_AMMO, startX, startY, dir, size, studentWorld, 100), m_studentWorld(studentWorld)
{
}
void Ammo::updatePlayerStat()
{
    m_studentWorld->addAmmo(20);
}