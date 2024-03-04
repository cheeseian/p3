#include "StudentWorld.h"
#include "GameConstants.h"
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>
#include <string>
using namespace std;

GameWorld *createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Constructor
StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_lev(assetPath), m_bonus(1000), m_crystalsLeft(0), m_quitLevel(false), m_levelComplete(false)
{
}

// Destructor
StudentWorld::~StudentWorld()
{
    cleanUp();
}

// Checks if the space (x,y) is walkable
bool StudentWorld::canMoveAt(double x, double y, int direction)
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if (((*it)->canWalkThrough()) == 0) // can't move through wall/robot/factory (0)
                return false;
            if (((*it)->canWalkThrough()) == 2) // marble (2)
                return tryMoveMarble((*it), direction);
        }
        it++;
    }
    return true;
}
// Checks if the space (x,y) is walkable
bool StudentWorld::robotCanMoveAt(double x, double y, int direction)
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if (((*it)->canWalkThrough()) == 0) // can't move through wall/robot/factory (0)
                return false;
            if (((*it)->canWalkThrough()) == 2) // marble (2)
                return false;
        }
        it++;
    }
    return true;
}

// Checks if a marble is able to be moved in a direction
bool StudentWorld::tryMoveMarble(Actor *marble, int direction)
{
    // check all bad cases, else return true
    // set x and y offsets
    int xOffset, yOffset;
    getOffsets(xOffset, yOffset, direction);
    // Iterate through all actors checking if any actors exist at a spot next to the marble in same direction
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end())
    {
        if ((*it)->getX() == (marble->getX() + xOffset) && (*it)->getY() == (marble->getY()) + yOffset)
        {
            // Checking for Actor that is right of the marble
            if ((*it)->canMarbleThrough() == 0) // can't move through wall/robot/factory (0)
                {
                    cerr << "can't move marble" << endl;
                    return false;
                }
        }
        it++;
    }
    marble->moveTo(marble->getX() + xOffset, marble->getY() + yOffset);

    return true;
}

// helper function for Pit
bool StudentWorld::checkIfMarbleAt(double x, double y)
{
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end())
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if((*it)->canWalkThrough() == 2) // is Marble
            {
                (*it)->setDead(); // kill marble
                return true;
            }
        }
        it++;
    }
    return false;
}

// takes a direction and outputs x and y offset by 1 in that direction, very useful!
void StudentWorld::getOffsets(int &xOffset, int &yOffset, int direction)
{
    switch (direction)
    {
    case 0:
        xOffset = 1;
        yOffset = 0;
        break;
    case 90:
        xOffset = 0;
        yOffset = 1;
        break;
    case 180:
        xOffset = -1;
        yOffset = 0;
        break;
    case 270:
        xOffset = 0;
        yOffset = -1;
        break;
    default:
        xOffset = 0;
        yOffset = 0;
    }
}

// Creates a Pea next to player/robot in direction. Adds to actors vector
void StudentWorld::shootPea(double x, double y, int direction)
{
    int xOffset, yOffset;
    getOffsets(xOffset, yOffset, direction);
    Pea *pea = new Pea(x + xOffset, y + yOffset, direction, 1, this);
    actors.push_back(pea);
}

// Checks the square a Pea is on, does respective actions
bool StudentWorld::checkPeaSquare(double x, double y)
{
    // if on top of player/robot, damage, die
    // if on top of wall or factory, just die

    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if((*it)->isCollideable() == 1) // hits a wall or factory
            {
                return true;
            }
            if((*it)->isCollideable() == 2) // hits a player or robot or marble
            {
                (*it)->recieveDamage();
                return true;
            }
        }
        it++;
    }
    return false;
}

// Checks if player is on square
bool StudentWorld::checkIfPlayerAt(double x, double y)
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->canControl())
        {
            if((*it)->getX() == x && (*it)->getY() == y)
                return true;
        }
        it++;
    }
    return false;
}

// Checks for robot if a player is in line of sight
bool StudentWorld::playerInView(double x, double y, int direction)
{
    int multiplier = 1;
    while (true)
    {
        int posX, posY;
        getOffsets(posX, posY, direction);
        posX *= multiplier; // allows us to step in direction 1 by 1
        posY *= multiplier; 

        vector<Actor *>::iterator it;
        it = actors.begin();
        while (it != actors.end())
        {
            // finding Actors in line of sight
            if ((*it)->getX() == x + posX && (*it)->getY() == y + posY) 
            {
                if (!(*it)->canShootThrough())
                {
                    // If can't walk through, we don't shoot
                    return false;
                }
                if ((*it)->canControl())
                {
                    // If it is player return true
                    cerr << "true" << endl;
                    return true;
                }
            }
            it++;
        }
        multiplier++;
    }
    return false;
}
// adds amount of ammo to player
void StudentWorld::addAmmo(int amt)
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->canControl())
        {
            (*it)->addPeas(amt);
        }
        it++;
    }
}
// adds amount of ammo to player
void StudentWorld::restoreHealth()
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->canControl())
        {
            (*it)->addHealth();
        }
        it++;
    }
}
// returns the amount of crystals left
int StudentWorld::crystalsLeft()
{
    return m_crystalsLeft;
}
// removes one crystal from the crystal counter
void StudentWorld::subtractCrystal()
{
    m_crystalsLeft--;
}
// Updates the on screen text
void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    int ammo, hp;

    // find the player
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        if ((*it)->canControl())
        {
            ammo = (*it)->ammoAmount();
            hp = (*it)->getHealth();
            break;
        }
        it++;
    }
    int health = ((hp * 100 )/ 20); 

    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
	oss << setw(7) << score;
    oss << "  Level: ";
    oss.fill('0');
    oss << setw(2) << level;
    oss << "  Lives: ";
    oss.fill(' ');
    oss << setw(2) << livesLeft;
    oss << "  Health: ";
    oss.fill(' ');
    oss << setw(3) << health;
    oss << "%";
    oss << "  Ammo: ";
    oss.fill(' ');
    oss << setw(3) << ammo;
    oss << "  Bonus: ";
    oss.fill(' ');
    oss << setw(4) << m_bonus;
	string s = oss.str();

    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}
// Deletes all actors that are dead and removes correspoinding pointer from vector
void StudentWorld::deleteDead()
{
    vector<Actor *>::iterator it;
    it = actors.begin();
    while (it != actors.end())
    {   
        if ((*it)->isDead())
        {
            (*it)->setVisible(false);
            delete *it;
            it = actors.erase(it);
            it = actors.begin();
        }
        it++;
    }

}
// If escape is pressed, lose life and restart level
void StudentWorld::escapePressed()
{
    decLives();
    m_quitLevel = true;
}
// If level is complete
void StudentWorld::levelComplete()
{
    increaseScore(2000);
    increaseScore(m_bonus);
    m_levelComplete = true;
}
// Generates correct string for level loading
string StudentWorld::getLevelString()
{
    int level = getLevel();
    ostringstream osslev;
    osslev << "level";
    osslev.fill('0');
	osslev << setw(2) << level;
    osslev << ".txt";

	return osslev.str();
}


int StudentWorld::init()
{
    m_quitLevel = false;
    m_levelComplete = false;

    // Loading current level
    //string curLevel = "level04.txt";
    string curLevel = getLevelString();
    Level::LoadResult result = m_lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level00.txt data file\n";
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted\n";
    else if (result == Level::load_success)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            for (int y = 0; y < VIEW_HEIGHT; y++)
            {
                Level::MazeEntry ge = m_lev.getContentsOf(x, y);
                switch (ge)
                {
                case Level::empty:
                    break;
                case Level::exit:
                {
                    Exit *exit = new Exit(x, y, -1, 1, this);
                    actors.push_back(exit);
                    break;
                }
                case Level::player:
                {
                    Avatar *avatar = new Avatar(x, y, 0, 1, this);
                    actors.push_back(avatar);
                    break;
                }
                case Level::horiz_ragebot:
                {
                    RageBot *rageBot = new RageBot(x, y, 0, 1, this);
                    actors.push_back(rageBot);
                    break;
                }
                case Level::vert_ragebot:
                {
                    RageBot *rageBot = new RageBot(x, y, 90, 1, this);
                    actors.push_back(rageBot);
                    break;
                }
                case Level::thiefbot_factory:
                {
                    ThiefBotFactory *botFactory = new ThiefBotFactory(x, y, -1, 1, false);
                    actors.push_back(botFactory);
                    break;
                }
                case Level::mean_thiefbot_factory:
                {
                    ThiefBotFactory *botFactory = new ThiefBotFactory(x, y, -1, 1, true);
                    actors.push_back(botFactory);
                    break;
                }
                case Level::wall:
                {
                    Wall *wall = new Wall(x, y, -1, 1);
                    actors.push_back(wall);
                    break;
                }
                case Level::marble:
                {
                    Marble *marble = new Marble(x, y, -1, 1);
                    actors.push_back(marble);
                    break;
                }
                case Level::pit:
                {
                    Pit *pit = new Pit(x, y, -1, 1, this);
                    actors.push_back(pit);
                    break;
                }
                case Level::crystal:
                {
                    Crystal *crystal = new Crystal(x, y, -1, 1, this);
                    actors.push_back(crystal);
                    m_crystalsLeft++;
                    break;
                }
                case Level::restore_health:
                {
                    RestoreHealth *restoreHealth = new RestoreHealth(x, y, -1, 1, this);
                    actors.push_back(restoreHealth);
                    break;
                }
                case Level::extra_life:
                {
                    ExtraLife *extraLife = new ExtraLife(x, y, -1, 1, this);
                    actors.push_back(extraLife);
                    break;
                }
                case Level::ammo:
                {
                    Ammo *ammo = new Ammo(x, y, -1, 1, this);
                    actors.push_back(ammo);
                    break;
                }
                }
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{   
    // check if escape pressed
    if (m_quitLevel)
        return GWSTATUS_PLAYER_DIED;
    if (m_levelComplete)
        return GWSTATUS_FINISHED_LEVEL;
    // Update Game Text
    setDisplayText();

    // Ask all actors to doSomething
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        (*it)->doSomething();
        it++;
    }

    // if player dead
    vector<Actor *>::iterator it2;
    it2 = actors.begin();
    while (it2 != actors.end())
    {
        if ((*it2)->canControl() && (*it2)->isDead())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        it2++;
    }

    // remove all dead actors
    deleteDead();
    
    // decrease bonus by 1
    if (m_bonus > 0)
        m_bonus--;

    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor *>::iterator it;
    it = actors.begin();

    while (it != actors.end())
    {
        delete *it;
        it = actors.erase(it);
    }
}


