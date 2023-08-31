#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "Board.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor (int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size);
    virtual void doSomething() = 0;
    virtual bool canBeShot() {return false;};
    StudentWorld* getWorld();
    bool isValidPos(int x,int y);
    virtual bool canLandOn() {return false;};
    virtual bool getActive() {return true;};
    bool getBeenImpacted();
    virtual bool canHurt() {return false;};
    void setBeenImpacted (bool setter);
    void teleport();
   virtual void setActive(bool setter){active = setter;};
    void setbeenTP(bool setter);
    bool getbeenTP();
    
private:
    StudentWorld* world;
    bool beenImpacted;
    bool beenTP;
    bool active;
};



//class Character, for peach, yoshi, bowser, etc (put walking dir in here)
class Character: public Actor
{
public:
    Character (int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size);
    virtual void doSomething() = 0;
    int getWalkingDir();
    void setWalkingDir(int dir);
    int getTicks();
    void setTicks (int ticks);
    void decrementTicks (int num);
    //False for Walking, True for Waiting
    bool getWalkingState();
    void setWalkingState(bool setter);
    //if can't move forward, choose dir perp, with up>down & right>left
    void chooseDirectionPerpendicular();
    //Move 2 pixels in walk dir
    void moveInWalkDir();
    //Is at a fork (multiple directions they can move)
    bool isAtFork();
    bool ableToMoveForward();
    
private:
    int walking_dir;
    int ticks_to_move;
    bool waiting_to_roll;
};

//peach and yoshi have same functionalities
class PlayerAvatar: public Character
{
public:
    PlayerAvatar (int imageID, int startX, int startY,int playerNum, StudentWorld* thisWorld);
    virtual void doSomething();
    void addCoins(int coinsAdded);
    int getCoins();
    void addStars(int starsAdded);
    int getStars();
    int getRoll();
    void setLandedOn (Board::GridEntry ge);
    Board::GridEntry getLandedOn();
    void setVortex (bool setter);
    bool getHasVortex ();
    bool getBeenBowsered();
    void setBeenBowsered( bool setter);
    //For Dropping Squares (to see if player has landed on a square)
    bool getPlayerHasLanded ();
    void setPlayerHasLanded(bool setter);
    bool getBeenBood();
    void setBeenBood(bool setter);
    void ifInvalidChooseRandDir ();
    bool getOnDirSquare();
    void setOnDirSquare(bool setter);
    
private:
    int dieRoll;
    int m_playerNum;
    int coins;
    int stars;
    Board::GridEntry squareLandedOn;
    //For Vortex
    bool hasVortex;
    //To prevent Bowser activation on the same square
    bool beenBowsered;
    bool hasLanded;
    bool beenBood;
    bool onDirSquare;
};

class Baddie : public Character
{
public:
    Baddie (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething () = 0;
    PlayerAvatar* getPlayer1();
    PlayerAvatar* getPlayer2();
    int getPauseCounter();
    void decrementPauseCounter (int num);
    void setPauseCounter(int num);
    int getSquaresToMove();
    void setSquaresToMove(int squares);
    //Chooses a random Dir for baddies (valid positions)
    void setRandomDir();
    virtual bool canHurt() {return true;};
    
private:
    PlayerAvatar* player1;
    PlayerAvatar* player2;
    //distance travelled in pixels
    int travelDist;
    //How long to wait on square before when finished walking
    int pauseCounter;
    int squares_to_move;
};

class Bowser : public Baddie
{
public:
    Bowser (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    
private:
};

class Boo : public Baddie
{
public:
    Boo (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    
private:
};

class Square: public Actor
{
public:
    Square(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2,int spriteDir, int depth, int size);
    PlayerAvatar* getPlayer1();
    PlayerAvatar* getPlayer2();
    bool playerIsOnExactSquare (Board::GridEntry ge, PlayerAvatar* player,Square* square);
    bool playerPassesBySquare (PlayerAvatar* player,Square* square);
    // adds/subtracts coins from player
    void changeCoins(int coins,PlayerAvatar* player);
    virtual bool canLandOn() {return true;};
    
private:
    PlayerAvatar* player1;
    PlayerAvatar* player2;
    
};

class CoinSquare : public Square
{
public:
    CoinSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    
private:
    bool alive; //Requirement 7
};

class StarSquare : public Square
{
public:
    StarSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    void changeStars(int stars, PlayerAvatar* player);
private:
    
};

class DirectionalSquare : public Square
{
public:
    DirectionalSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2,int spriteDir, int depth, int size, int dir);
    virtual void doSomething();
    void setDir(int dir, PlayerAvatar* player);
private:
    int direction;
};

class BankSquare: public Square
{
public:
    BankSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    void bankTakesCoins (PlayerAvatar* player);
    void giveAllCoins (PlayerAvatar* player);
private:
    
};

class DroppingSquare : public Square
{
public:
    DroppingSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    void deductCoins(PlayerAvatar* player);
    void deductStars (PlayerAvatar* player);
    
private:
};

class EventSquare : public Square
{
public:
    EventSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2);
    virtual void doSomething();
    void teleport (PlayerAvatar* player);
    void swapPlayers (PlayerAvatar* playerWhoLandedOnSquare);
    void giveVortex (PlayerAvatar* player);
    
private:
};

class Vortex: public Character
{
public:
    Vortex (int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size, int direction);
    virtual void doSomething();
    virtual bool getActive();
    void setActive(bool setter);
    //void teleport (Actor* actor);
    virtual bool canBeShot() {return true;};
    
private:
    bool active;

};

#endif // ACTOR_H_
