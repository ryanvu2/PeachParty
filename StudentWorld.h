#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    Board getBoard();
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    void addBankTotal(int num);
    int getBankTotal();
    void emptyBankTotal();
    void deleteSquareAt (int x, int y);
    void insertDroppingSquareAt (int x, int y);
    void createVortex(int x, int y, int direction);
    void setVortexCreated(bool setter);
    bool peachWon();
    bool checkVortexOverlap();
    ~StudentWorld();

private:
    Board bd;
    //keep track of where actors are here?
    std::vector <Actor*> actors;
    std::vector <PlayerAvatar*> avatars;
    std::string boardNum;
    int bankTotal;
    bool vortexCreated;
};

#endif // STUDENTWORLD_H_
