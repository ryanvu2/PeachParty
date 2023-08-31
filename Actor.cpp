#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include <cstdlib>

Actor :: Actor(int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size) : GraphObject(imageID, startX, startY, spriteDir, depth, size)
{
    world = thisWorld;
    beenImpacted = false;
    beenTP = false;
    active = false;
}

bool Actor::getBeenImpacted()
{
    return beenImpacted;
}

bool Actor::getbeenTP()
{
    return beenTP;
}

void Actor::setbeenTP(bool setter)
{
    beenTP = setter;
}

void Actor::setBeenImpacted(bool setter)
{
    beenImpacted = setter;
}

StudentWorld* Actor:: getWorld()
{
    return world;
}

int Character:: getWalkingDir()
{
    return walking_dir;
}

void Character:: setWalkingDir(int dir)
{
    walking_dir = dir;
}

int Character:: getTicks()
{
    return ticks_to_move;
}

void Character::setTicks (int ticks)
{
    ticks_to_move = ticks;
}

void Character::decrementTicks (int num)
{
    ticks_to_move -= num;
}

Character :: Character (int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size) : Actor (imageID, startX, startY, thisWorld, spriteDir, depth, size)
{
    walking_dir = right;
    ticks_to_move =0;
    //Starts in Paused state for Baddies
    waiting_to_roll = true;
}

void Character::moveInWalkDir()
{
    switch (getWalkingDir())
    {
        case left:
            setDirection(left);
            moveTo(getX()-2, getY());
            break;
        case right:
            setDirection(right);
            moveTo(getX()+2, getY());
            break;
        case up:
            setDirection(right);
            moveTo(getX(), getY()+2);
            break;
        case down:
            setDirection(right);
            moveTo(getX(), getY()-2);
            break;
    }
}

bool Character:: isAtFork()
{
//    if (ableToMoveForward() == false)
//        return false;
    if (getX() != (SPRITE_WIDTH * (getX() /SPRITE_WIDTH)) || getY() != (SPRITE_HEIGHT * (getY() /SPRITE_HEIGHT)))
    {
        return false;
    }
    int numberOfDirections = 0;
    //if able to go up
    if(isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT +1) == true)
        numberOfDirections++;
    //down
    if (isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT -1) == true)
        numberOfDirections++;
    //if able to go right
    if(isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT ) == true)
        numberOfDirections++;
    //left
    if (isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == true)
        numberOfDirections++;
    
    return numberOfDirections > 2;
}

void Character::chooseDirectionPerpendicular()
{
    //If can't continue moving forward in current dir, change dir
        //if facing right and can't move forward
        if (getTicks() % 8 == 0 && getWalkingDir() == right && isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT) == false)
        {
            //if able to go up
            if(isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT +1) == true)
                setWalkingDir(up);
            //if able to go down
            else if (isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT -1) == true)
            {
                setWalkingDir(down);
            }
        }
        //if facing left and can't move forward
        else if (getTicks() % 8 == 0 && getWalkingDir() == left && isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == false)
        {
            //if able to go up
            if(isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT +1) == true)
                setWalkingDir(up);
            //if able to go down
            else if (isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT -1 )== true)
            {
                setWalkingDir(down);
            }
        }
        //if facing up and can't move forward
        else if (getTicks() % 8 ==0 && getWalkingDir() == up && isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT + 1) == false)
        {
            //if able to go right
            if(isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT ) == true)
                setWalkingDir(right);
            else if (isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == true)
            {
                setWalkingDir(left);
            }
        }
        //if facing down and can't move forward
        
        else if (getTicks() % 8 ==0 && getWalkingDir() == down && isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT - 1) == false)
        {
            //if able to go right
            //Directional Square Problem: is saying down is a valid Pos and passing it here
            if(isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT ) == true)
                setWalkingDir(right);
            //if able to go left
            else if (isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == true)
            {
                setWalkingDir(left);
            }
        }
}

Baddie:: Baddie (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2) : Character(imageID, startX, startY, thisWorld, right, 0, 1)
{
    travelDist = 0;
    pauseCounter = 180;
    player1 = p1;
    player2 = p2;
}

int Baddie :: getPauseCounter()
{
    return pauseCounter;
}

void Baddie :: decrementPauseCounter(int num)
{
    pauseCounter -= num;
}

int Baddie:: getSquaresToMove()
{
    return squares_to_move;
}

void Baddie :: setSquaresToMove(int squares)
{
    squares_to_move = squares;
}

void Baddie :: setRandomDir()
{
    //Is Bowser's walking dir legal?
    bool isLegal = false;
//Chooing Bowser's direction
    while (isLegal == false)
    {
        int randDir = randInt(1, 4);
        switch (randDir)
        {
                //up
            case 1:
                if (isValidPos(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT +1) )
                {
                    setWalkingDir(up);
                    setDirection(right);
                    isLegal = true;
                }
                break;
                //down
            case 2:
                if (isValidPos(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT -1) )
                {
                    setWalkingDir(down);
                    setDirection(right);
                    isLegal = true;
                }
                break;
                //right
            case 3:
                if (isValidPos(getX()/SPRITE_WIDTH+1, getY()/SPRITE_HEIGHT ) )
                {
                    setWalkingDir(right);
                    setDirection(right);
                    isLegal = true;
                }
                break;
                //left
            case 4:
                if (isValidPos(getX()/SPRITE_WIDTH-1, getY()/SPRITE_HEIGHT ) )
                {
                    setWalkingDir(left);
                    setDirection(left);
                    isLegal = true;
                }
                break;
        }
    }
}

bool Character:: ableToMoveForward()
{
    if (getX() != (SPRITE_WIDTH * (getX() /SPRITE_WIDTH)) || getY() != (SPRITE_HEIGHT * (getY() /SPRITE_HEIGHT)))
    {
        return false;
    }
    int currentDir = getWalkingDir();
    switch (currentDir)
    {
        case right:
            if(isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT ) == true)
                return true;
            break;
        case left:
            if (isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == true)
                return true;
            break;
        case up:
            if(isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT +1) == true)
                return true;
            break;
        case down:
            if(isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT -1 )== true)
                return true;
            break;
    }
    return false;
}

void Baddie::setPauseCounter(int num)
{
    pauseCounter = num;
}

PlayerAvatar* Baddie:: getPlayer1()
{
    return player1;
}

PlayerAvatar* Baddie:: getPlayer2()
{
    return player2;
}


Bowser:: Bowser (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2) : Baddie (imageID, startX, startY, thisWorld, p1, p2)
{
    
}

Boo:: Boo (int imageID, int startX, int startY, StudentWorld* thisWorld, PlayerAvatar* p1, PlayerAvatar* p2) : Baddie (imageID, startX, startY, thisWorld, p1, p2)
{
    
}

void Boo:: doSomething()
{
//If Boo is in Paused State
    if (getWalkingState() == true)
    {
        //if its been impacted by vortex
        if (getBeenImpacted() == true)
        {
            setWalkingDir(right);
            setDirection(0);
            //set to paused state
            setWalkingState(true);
            //Pause ticks to 180
            setPauseCounter(180);
            
            //so it doesn't keep being impacted
            setBeenImpacted(false);
            return;
        }
        //if player hasn't beenBood at this location && player is waiting && lands on Boo
        if (getPlayer1()->getBeenBood() == false && getPlayer1()->getWalkingState() == true && getPlayer1()->getX() == getX() && getPlayer1()->getY() == getY())
        {
            int option = randInt(1, 2);
            switch (option)
            {
                    //swap coins
                case 1:
                {
                    int tempCoins = getPlayer1()->getCoins();
                    //Gives p2's coins to p1
                    getPlayer1()->addCoins(0 - getPlayer1()->getCoins() + getPlayer2()-> getCoins());
                    //Gives p1's coins to p2
                    getPlayer2()->addCoins(0 - getPlayer2()->getCoins() + tempCoins);
                    break;
                }
                    //swap stars
                case 2:
                {
                    int tempStars = getPlayer1()->getStars();
                    //Gives p2's coins to p1
                    getPlayer1()->addStars(0 - getPlayer1()->getStars() + getPlayer2()-> getStars());
                    //Gives p1's coins to p2
                    getPlayer2()->addStars(0 - getPlayer2()->getStars() + tempStars);
                    break;
                }
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
            getPlayer1()->setBeenBood(true);
        }
        if (getPlayer2()->getBeenBood() == false && getPlayer2()->getWalkingState() == true && getPlayer2()->getX() == getX() && getPlayer2()->getY() == getY())
        {
            int option = randInt(1, 2);
            switch (option)
            {
                    //swap coins
                case 1:
                {
                    int tempCoins = getPlayer1()->getCoins();
                    //Gives p2's coins to p1
                    getPlayer1()->addCoins(0 - getPlayer1()->getCoins() + getPlayer2()-> getCoins());
                    //Gives p1's coins to p2
                    getPlayer2()->addCoins(0 - getPlayer2()->getCoins() + tempCoins);
                    break;
                }
                    //swap stars
                case 2:
                {
                    int tempStars = getPlayer1()->getStars();
                    //Gives p2's coins to p1
                    getPlayer1()->addStars(0 - getPlayer1()->getStars() + getPlayer2()-> getStars());
                    //Gives p1's coins to p2
                    getPlayer2()->addStars(0 - getPlayer2()->getStars() + tempStars);
                    break;
                }
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
            getPlayer2()->setBeenBood(true);
        }
        //Decrement pause counter
        decrementPauseCounter(1);
        if (getPauseCounter() == 0)
        {
            int squares = randInt(1, 3);
            setSquaresToMove(squares);
            setTicks(getSquaresToMove() * 8);
            setRandomDir();

        //set Boo to Walking State
            setWalkingState(false);
        }
    }
//Step 2 (If Boo is In walking State)
    if (getWalkingState() == false)
    {
        //if its been impacted by vortex
        if (getBeenImpacted() == true)
        {
            setWalkingDir(right);
            setDirection(0);
            //set to paused state
            setWalkingState(true);
            //Pause ticks to 180
            setPauseCounter(180);
            
            //so it doesn't keep being impacted
            setBeenImpacted(false);
            return;
        }
        //if Boo is at a fork
        if (isAtFork() == true )
        {
            setRandomDir();
        }
        //if Boo is not at a fork (there is not a square in front of Boo)
        else if (ableToMoveForward() != true)
        {
            chooseDirectionPerpendicular();
        }
        //move 2 pixels in walk dir
        moveInWalkDir();
        decrementTicks(1);
        if (getTicks() == 0)
        {
            //When Boo moves (allow Players to get Bood again)
            getPlayer1() -> setBeenBood(false);
            getPlayer2() -> setBeenBood(false);
            //set to paused state
            setWalkingState(true);
            setPauseCounter(180);
        }
    }
}

void Bowser :: doSomething()
{
//If Bowser is in Paused State
    if (getWalkingState() == true)
    {
        //if its been impacted by vortex
        if (getBeenImpacted() == true)
        {
            setWalkingDir(right);
            setDirection(0);
            //set to paused state
            setWalkingState(true);
            //Pause ticks to 180
            setPauseCounter(180);
            
            //so it doesn't keep being impacted
            setBeenImpacted(false);
            return;
        }
        //if player hasn't beenBowsered at this location && player is waiting && lands on Bowser
        if (getPlayer1()->getBeenBowsered() == false && getPlayer1()->getWalkingState() == true && getPlayer1()->getX() == getX() && getPlayer1()->getY() == getY())
        {
            int option = randInt(1, 2);
            switch (option)
            {
                    // loses all coins
                case 1:
                    getPlayer1()->addCoins(0 - getPlayer1()->getCoins());
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    break;
                case 2:
                    break;
            }
            getPlayer1()->setBeenBowsered(true);
        }
        if (getPlayer2()->getBeenBowsered() == false && getPlayer2()->getWalkingState() == true && getPlayer2()->getX() == getX() && getPlayer2()->getY() == getY())
        {
            int option = randInt(1, 2);
            switch (option)
            {
                    // loses all coins
                case 1:
                    getPlayer2()->addCoins(0 - getPlayer2()->getCoins());
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                    break;
                case 2:
                    break;
            }
            getPlayer2()->setBeenBowsered(true);
        }
        //Decrement pause counter
        decrementPauseCounter(1);
        if (getPauseCounter() == 0)
        {
            int squares = randInt(1, 10);
            setSquaresToMove(squares);
            setTicks(getSquaresToMove() * 8);
            setRandomDir();
            //std::cerr<< "Pause change dir"<<std::endl;
        //set Bowser to Walking State
            setWalkingState(false);
        }
    }
//Step 2 (If Bowser is In walking State)
    if (getWalkingState() == false)
    {
        //if its been impacted by vortex
        if (getBeenImpacted() == true)
        {
            setWalkingDir(right);
            setDirection(0);
            //set to paused state
            setWalkingState(true);
            //Pause ticks to 180
            setPauseCounter(180);
            
            //so it doesn't keep being impacted
            setBeenImpacted(false);
            return;
        }
        //if Bowser is at a fork (there is a square in front of Bowser essentially)
        if (isAtFork() == true )
        {
            setRandomDir();
            //std::cerr<< "Fork Change Dir"<<std::endl;
        }
        //if Bowser is not at a fork (there is not a square in front of Bowser)
        else if (ableToMoveForward() != true)
        {
            chooseDirectionPerpendicular();
            //std::cerr<< "Forward Change Dir"<<std::endl;
        }
        //move 2 pixels in walk dir
        moveInWalkDir();
        decrementTicks(1);
        if (getTicks() == 0)
        {
            //When Bowser moves (allow Players to get Bowsered again)
            getPlayer1() -> setBeenBowsered(false);
            getPlayer2() -> setBeenBowsered(false);
            //set to paused state
            setWalkingState(true);
            setPauseCounter(180);
            //25% chance of dropping square
            int chance = randInt(1, 4);
            switch (chance)
            {
                //1 out of 4 chance that it happens
                case 1:
                //Find Current Square and delete it
                    getWorld()->deleteSquareAt(getX(), getY());
                //Insert new Dropping Square
                    getWorld()->insertDroppingSquareAt(getX(), getY());
                    getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }
        }
    }
}

PlayerAvatar :: PlayerAvatar (int imageID, int startX, int startY, int playerNum, StudentWorld* thisWorld) : Character(imageID, startX, startY, thisWorld, 0, 0, 1)
{
    coins = 0;
    stars = 0;
    dieRoll = 0;
    m_playerNum = playerNum;
    hasVortex =false;
    beenBowsered = false;
    hasLanded = false;
    beenBood = false;
}

bool PlayerAvatar::getBeenBood()
{
    return beenBood;
}

void PlayerAvatar::setBeenBood(bool setter)
{
    beenBood = setter;
}


bool PlayerAvatar::getBeenBowsered()
{
    return beenBowsered;
}

void PlayerAvatar::setBeenBowsered(bool setter)
{
    beenBowsered = setter;
}

void PlayerAvatar::addCoins(int coinsAdded)
{
    coins += coinsAdded;
}

int PlayerAvatar::getCoins()
{
    return coins;
}

void PlayerAvatar::addStars(int starsAdded)
{
    stars += starsAdded;
}

int PlayerAvatar::getStars()
{
    return stars;
}

int PlayerAvatar::getRoll()
{
    return dieRoll;
}

void PlayerAvatar::setLandedOn (Board::GridEntry ge)
{
    squareLandedOn = ge;
}

Board::GridEntry PlayerAvatar::getLandedOn()
{
    return squareLandedOn;
}

bool Character:: getWalkingState()
{
    return waiting_to_roll;
}

void Character::setWalkingState(bool setter)
{
    waiting_to_roll = setter;
}

void PlayerAvatar::setVortex(bool setter)
{
    hasVortex = setter;
}

bool PlayerAvatar::getHasVortex()
{
    return hasVortex;
}

Square :: Square (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2, int spriteDir, int depth, int size) : Actor(imageID, startX, startY, thisWorld, spriteDir, depth, size)
{
    player1 = p1;
    player2 =p2;
}

bool PlayerAvatar :: getPlayerHasLanded()
{
    return hasLanded;
}

void PlayerAvatar :: setPlayerHasLanded(bool setter)
{
    hasLanded = setter;
}

CoinSquare :: CoinSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2) : Square(imageID, startX, startY, thisWorld, p1 ,p2, right,1,1)
{
    alive = true;
}

DirectionalSquare :: DirectionalSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2, int spriteDir, int depth, int size, int dir) : Square(imageID, startX, startY, thisWorld, p1 ,p2,spriteDir, 1,1)
{
    direction = dir;
}

BankSquare:: BankSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2) : Square(imageID, startX, startY, thisWorld, p1 ,p2, right,1,1)
{
    
}

DroppingSquare:: DroppingSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2) : Square(imageID, startX, startY, thisWorld, p1 ,p2, right,1,1)
{
    
}

EventSquare :: EventSquare(int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2) : Square(imageID, startX, startY, thisWorld, p1 ,p2, right,1,1)
{
    
}

//informs player that they will be teleported to random square
void EventSquare::teleport(PlayerAvatar *player)
{
    bool hasBeenTeleported = false;
    while (hasBeenTeleported == false)
    {
        int x = randInt(0, 15);
        int y = randInt(0, 15);
        if (isValidPos(x, y))
        {
            player->moveTo(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            hasBeenTeleported = true;
        }
    }
}

void EventSquare:: swapPlayers(PlayerAvatar *playerWhoLandedOnSquare)
{
    //swap x,y
    int tempX = getPlayer1()->getX();
    int tempY = getPlayer1() -> getY();
    getPlayer1()->moveTo(getPlayer2()->getX(), getPlayer2()->getY());
    getPlayer2()-> moveTo(tempX, tempY);
    
    //swap number of ticks
    int tempTicks = getPlayer1()->getTicks();
    getPlayer1()->setTicks(getPlayer2()->getTicks());
    getPlayer2()->setTicks(tempTicks);
    
    //swap walk dir
    int tempWalkingDir = getPlayer1()->getWalkingDir();
    getPlayer1()->setWalkingDir(getPlayer2()->getWalkingDir());
    getPlayer2()->setWalkingDir(tempWalkingDir);
    
    //swap sprite dir
    int tempSpriteDir = getPlayer1()->getDirection();
    getPlayer1()->setDirection(getPlayer2()->getDirection());
    getPlayer2()->setDirection(tempSpriteDir);
    
    //swap roll/walk state
    bool tempWalkingState = getPlayer1()->getWalkingState();
    getPlayer1()->setWalkingState(getPlayer2()->getWalkingState());
    getPlayer2()->setWalkingState(tempWalkingState);
    
    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}

void EventSquare:: giveVortex(PlayerAvatar *player)
{
    player->setVortex (true);
    getWorld()->playSound(SOUND_GIVE_VORTEX);
}

void EventSquare::doSomething()
{
    if (playerIsOnExactSquare(Board::event_square, getPlayer1(), this))
    {
        int randomOption = randInt(1, 3);
            switch (randomOption)
            {
                case 1:
                    teleport(getPlayer1());
                    break;
                case 2:
                    swapPlayers(getPlayer1());
                    break;
                case 3:
                    giveVortex(getPlayer1());
                    break;
            }
    }
    if (playerIsOnExactSquare(Board::event_square, getPlayer2(), this))
    {
        int randomOption = randInt(1, 3);
        switch (randomOption)
        {
            case 1:
                teleport(getPlayer2());
                break;
            case 2:
                swapPlayers(getPlayer2());
                break;
            case 3:
                giveVortex(getPlayer2());
                break;
        }
    }
    
}

void DroppingSquare:: deductCoins(PlayerAvatar* player)
{
    if (player->getCoins() < 10)
        player-> addCoins(0 - player->getCoins());
    else
        player-> addCoins(-10);
}

void DroppingSquare:: deductStars(PlayerAvatar *player)
{
    if (player-> getStars() < 1)
        return;
    else
        player-> addStars(-1);
}

void DroppingSquare::doSomething()
{
    if (getPlayer1()->getPlayerHasLanded() == true && getPlayer1()->getX() == this->getX() && getPlayer1()->getY() == this->getY()  )
    {
        int randomOption1 = randInt(1, 2);
        switch (randomOption1)
        {
            case 1:
                deductCoins(getPlayer1());
                break;
            case 2:
                deductStars(getPlayer1());
                break;
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
    if (getPlayer2()->getPlayerHasLanded() == true && getPlayer2()->getX() == this->getX() && getPlayer2()->getY() == this->getY() )
    {
        int randomOption2 = randInt(1, 2);
        switch (randomOption2)
        {
            case 1:
                deductCoins(getPlayer2());
                break;
            case 2:
                deductStars(getPlayer2());
                break;
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}

void BankSquare::bankTakesCoins(PlayerAvatar *player)
{
    if (player->getCoins() < 5)
    {
        getWorld()->addBankTotal(player->getCoins());
        player->addCoins(0 - player->getCoins());
    }
    else
    {
        getWorld()->addBankTotal(5);
        player->addCoins(-5);
    }
    getWorld()->playSound(SOUND_DEPOSIT_BANK);
}

void BankSquare::giveAllCoins(PlayerAvatar *player)
{
    int bankTotal = getWorld()->getBankTotal();
    player->addCoins(bankTotal);
    getWorld()->emptyBankTotal();
    getWorld()->playSound(SOUND_WITHDRAW_BANK);
}

void BankSquare::doSomething()
{
    if (playerIsOnExactSquare(Board::bank_square, getPlayer1(), this))
    {
        giveAllCoins(getPlayer1());
    }
    else if (playerPassesBySquare(getPlayer1(), this))
    {
        bankTakesCoins(getPlayer1());
    }
    if (playerIsOnExactSquare(Board::bank_square, getPlayer2(), this))
    {
        giveAllCoins(getPlayer2());
    }
    else if (playerPassesBySquare(getPlayer2(), this))
    {
        bankTakesCoins(getPlayer2());
    }
}

void DirectionalSquare::setDir(int dir, PlayerAvatar *player)
{
    player -> setWalkingDir(dir);
    if (player->getWalkingDir() == left)
        player-> setDirection(left);
    else
        player-> setDirection(right);
}

void DirectionalSquare::doSomething()
{
    //just needs to touch the square, not land on it
    if (playerPassesBySquare(getPlayer1(), this))
    {
        setDir(direction, getPlayer1());
        getPlayer1()->setOnDirSquare(true);
    }
    if (playerPassesBySquare(getPlayer2(), this))
    {
        setDir(direction, getPlayer2());
        getPlayer2()->setOnDirSquare(true);
    }
}

void Square::changeCoins(int coins, PlayerAvatar* player)
{
    player->addCoins(coins);
}

StarSquare :: StarSquare (int imageID, int startX, int startY, StudentWorld* thisWorld,PlayerAvatar* p1, PlayerAvatar* p2) : Square(imageID, startX, startY, thisWorld, p1 ,p2,right,1,1)
{
}

void StarSquare::changeStars(int stars, PlayerAvatar* player)
{
    player->addStars(stars);
}

PlayerAvatar* Square:: getPlayer1()
{
    return player1;
}
PlayerAvatar* Square:: getPlayer2()
{
    return player2;
}

//checks if player is on a square of certain SquareType, and has same coordinates as that square
bool Square::playerIsOnExactSquare (Board::GridEntry ge, PlayerAvatar* player,Square* square)
{
    if(player->getLandedOn() == ge && player->getX() == square->getX() && player->getY() == square->getY())
    {
        return true;
    }
    return false;
}
             
bool Square::playerPassesBySquare (PlayerAvatar* player,Square* square)
{
    if(player->getX() == square->getX() && player->getY() == square->getY())
    {
        return true;
    }
        return false;
}

//SHOULD they get coins from landing on the starting square (player square)
void CoinSquare :: doSomething()
{
    if (alive == false)
        return;
    
    //if landed on blue coin square
    if(playerIsOnExactSquare (Board::blue_coin_square, getPlayer1(),this))
    {
        changeCoins (3,getPlayer1());
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    if(playerIsOnExactSquare (Board::blue_coin_square, getPlayer2(),this))
    {
        changeCoins (3,getPlayer2());
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    //if landed on red coin square
    if(playerIsOnExactSquare (Board::red_coin_square, getPlayer1(),this))
    {
        if (getPlayer1()->getCoins() < 3)
            changeCoins(0 - getPlayer1()->getCoins(), getPlayer1());
        else
            changeCoins (-3,getPlayer1());
        
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
    if(playerIsOnExactSquare (Board::red_coin_square, getPlayer2(),this))
    {
        if (getPlayer2()->getCoins() < 3)
            changeCoins(0 - getPlayer2()->getCoins(), getPlayer2());
        else
            changeCoins (-3,getPlayer2());
        
        getWorld()->playSound(SOUND_TAKE_COIN);
    }
    
    //if landed on player square (starting square)
    if(playerIsOnExactSquare (Board::player, getPlayer1(),this))
    {
        changeCoins (3,getPlayer1());
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    if(playerIsOnExactSquare (Board::player, getPlayer2(),this))
    {
        changeCoins (3,getPlayer2());
        getWorld()->playSound(SOUND_GIVE_COIN);
    }
    
}

void StarSquare::doSomething()
{
    //if landed on star square OR walks over it
    if(playerIsOnExactSquare (Board::star_square, getPlayer1(),this) || playerPassesBySquare(getPlayer1(), this))
    {
        if (getPlayer1()->getCoins() < 20)
            return;
        else
        {
            changeCoins(-20, getPlayer1());
            changeStars(1,getPlayer1());
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
    if(playerIsOnExactSquare (Board::star_square, getPlayer2(),this)|| playerPassesBySquare(getPlayer2(), this))
    {
        if (getPlayer2()->getCoins() < 20)
            return;
        else
        {
            changeCoins(-20, getPlayer2());
            changeStars(1,getPlayer2());
            getWorld()->playSound(SOUND_GIVE_STAR);
        }
    }
}

//enter parameters NOT IN PIXELS (0-15)
bool Actor::isValidPos(int x,int y)
{
    Board::GridEntry ge = world->getBoard().getContentsOf(x, y);
    
    if (ge == Board::empty)
        return false;
    
    return true;
}

void PlayerAvatar::setOnDirSquare(bool setter)
{
    onDirSquare = setter;
}

bool PlayerAvatar::getOnDirSquare()
{
    return onDirSquare;
}

void PlayerAvatar:: ifInvalidChooseRandDir ()
{
    int rand = randInt(1, 4);
    switch (rand)
    {
        case 1:
            rand = right;
            break;
        case 2:
            rand = left;
            break;
        case 3:
            rand = up;
            break;
        case 4:
            rand = down;
            break;
    }
    //If avatar has invalid dir (due to being teleported)

//keep setting direction until its valid
    while (true)
    {
        //if facing right and CAN move forward
        if (getTicks() % 8 == 0 && getWalkingDir() == right && isValidPos(getX()/SPRITE_WIDTH +1,getY()/SPRITE_HEIGHT) == true)
        {
            setbeenTP(false);
            return;
        }
        //if facing left and CAN move forward
        else if (getTicks() % 8 == 0 && getWalkingDir() == left && isValidPos(getX()/SPRITE_WIDTH -1,getY()/SPRITE_HEIGHT) == true)
        {
            setbeenTP(false);
            return;
        }
        //if facing up and CAN move forward
        else if (getTicks() % 8 ==0 && getWalkingDir() == up && isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT + 1) == true)
        {
            setbeenTP(false);
            return;
        }
        //if facing down and CAN move forward
        else if (getTicks() % 8 ==0 && getWalkingDir() == down && isValidPos(getX()/SPRITE_WIDTH ,getY()/SPRITE_HEIGHT - 1) == true)
        {
            setbeenTP(false);
            return;
        }
        setWalkingDir(rand);
    }
}

void PlayerAvatar :: doSomething()
{
    //refreshes landing square (so that coin squares dont constantly pile up coins)
    setLandedOn(Board::empty);
    //Gets the ACTUAL type of square avatar is on
    Board::GridEntry ge = getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT);
    
//if player is waiting to roll
    if (getWalkingState() == true)
    {
        if (ge == Board::bank_square)
        {
            setLandedOn(ge);
        }
    //If avatar has invalid dir (due to being teleported)
    if (getbeenTP() == true)
    {
        ifInvalidChooseRandDir();
        
        if (getWalkingDir() == left)
            setDirection(left);
        else
            setDirection(right);
    }
    //See if user pressed key using getAction()
        switch (getWorld()->getAction(m_playerNum))
        {
            case ACTION_NONE:
                break;
            case ACTION_ROLL:
            {
                dieRoll = randInt(1, 10);
                setTicks(dieRoll * 8);
                setWalkingState(false);
                break;
            }
            case ACTION_FIRE:
            {
                //Needs to check if player has a vortex first?
            if (getHasVortex() == true)
            {
                int adderX = 0;
                int adderY = 0;
                int dir = getWalkingDir();
                switch (dir)
                {
                    case right:
                        adderX = 16;
                        break;
                    case left:
                        adderX = -16;
                        break;
                    case up:
                        adderY = 16;
                        break;
                    case down:
                        adderY = -16;
                        break;
                }
                getWorld()->createVortex(getX()+adderX, getY()+adderY, getWalkingDir());
                getWorld()->setVortexCreated(true);
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                setVortex(false);
            }
                break;
        }
        default:
                 return;
         }
    }

//this means they are in walking state
    if (getWalkingState() == false)
    {
        //player is now on the move, not landed on a square
        setPlayerHasLanded(false);
        
    //if at fork
        if (isAtFork() == true && onDirSquare == false)
        {
            int currentWalkingDir = getWalkingDir();
            int currentDir = getDirection();
        //check if user pressed a key using getAction()
            switch (getWorld()->getAction(m_playerNum))
            {
                case ACTION_UP:
                    if (currentWalkingDir == down)
                        return;
                    
                    setWalkingDir(up);
                    setDirection(right);
                    if (ableToMoveForward() != true)
                    {
                        setWalkingDir(currentWalkingDir);
                        setDirection(currentDir);
                        return;
                    }
                    break;
                case ACTION_DOWN:
                    if (currentWalkingDir == up)
                        return;
                    setWalkingDir(down);
                    setDirection(right);
                    if (ableToMoveForward() != true)
                    {
                        setWalkingDir(currentWalkingDir);
                        setDirection(currentDir);
                        return;
                    }
                    break;
                case ACTION_LEFT:
                    if (currentWalkingDir == right)
                        return;
                    setWalkingDir(left);
                    setDirection(left);
                    if (ableToMoveForward() != true)
                    {
                        setWalkingDir(currentWalkingDir);
                        setDirection(currentDir);
                        return;
                    }
                    break;
                case ACTION_RIGHT:
                    if (currentWalkingDir == left)
                        return;
                    setWalkingDir(right);
                    setDirection(right);
                    if (ableToMoveForward() != true)
                    {
                        setWalkingDir(currentWalkingDir);
                        setDirection(currentDir);
                        return;
                    }
                    break;
                default:
                    return;
            }
        }
        
        else if (ableToMoveForward() != true)
        {
            chooseDirectionPerpendicular();
        }
        //Update dir sprite faces && Move 2 pixels in walk dir
        moveInWalkDir();
        //after moving, no longer on dir square
        onDirSquare = false;
        decrementTicks(1);
        if (getTicks() % 8 == 0)
        {
            //For the game statistics display
            dieRoll-=1;
        }
        
        //here is where the player lands?
        if (getTicks() == 0)
        {
            setWalkingState(true);
            ge = getWorld()->getBoard().getContentsOf(getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT);
            //type of block player lands on
            setLandedOn(ge);
            setPlayerHasLanded(true);
        }
    }
}

Vortex::Vortex (int imageID, int startX, int startY, StudentWorld* thisWorld, int spriteDir, int depth, int size, int direction) : Character (imageID, startX, startY, thisWorld, spriteDir, depth, size)
{
    active = true;
    setWalkingDir(direction);
}

bool Vortex::getActive()
{
    return active;
}

void Vortex :: setActive(bool setter)
{
    active = setter;
}

void Actor:: teleport()
{
    bool beenTeleported = false;
    while (beenTeleported == false)
    {
        int x = randInt(0, 15);
        int y = randInt(0, 15);
        if (isValidPos(x, y))
        {
            moveTo(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
            beenTeleported = true;
            setbeenTP (true);
        }
    }
    
    //tell baddie that its been impacted
    setBeenImpacted(true);
}

void Vortex :: doSomething()
{
    //Vortex not active
    if (getActive() == false)
        return;
    
    //Move two pixels in current movement dir
    moveInWalkDir();
    
    //Vortex leaves the screen
    if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
        setActive(false);
    
    getWorld()->checkVortexOverlap();
    if (getWorld()->checkVortexOverlap() == true)
    {
        setActive(false);
        return;
    }
}


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
