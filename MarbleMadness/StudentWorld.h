#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  void deleteDead();
  bool canMoveAt(double x, double y, int direction);
  bool robotCanMoveAt(double x, double y, int direction);
  bool tryMoveMarble(Actor* marble, int direction);
  bool checkIfMarbleAt(double x, double y);
  void getOffsets(int &xOffset, int &yOffset, int direction);
  void shootPea(double x, double y, int direction);
  bool checkPeaSquare(double x, double y);
  bool checkIfPlayerAt(double x, double y);
  bool playerInView(double x, double y, int direction);
  void addAmmo(int amt);
  void restoreHealth();
  int crystalsLeft();
  void subtractCrystal();
  void setDisplayText();

private:
  vector<Actor*> actors;
  Level m_lev;
  int m_bonus;
  int m_crystalsLeft;

};

#endif // STUDENTWORLD_H_
