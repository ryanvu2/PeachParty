#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    bankTotal =0;

}

void StudentWorld:: setVortexCreated(bool setter)
{
    vortexCreated = setter;
}

int StudentWorld::getBankTotal()
{
    return bankTotal;
}

void StudentWorld::addBankTotal(int num)
{
    bankTotal +=num;
}

void StudentWorld::emptyBankTotal()
{
    bankTotal = 0;
}

Board StudentWorld::getBoard()
{
    return bd;
}

//create a new object here to get it on the game board
// look at the demo zip file
int StudentWorld::init()
{

    boardNum = to_string(getBoardNumber());
    string board_file = assetPath() + "board0" + boardNum + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_fail_bad_format)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_success)
        cerr << "Successfully loaded board\n";
    
    //construct players first (so they can be used as parameters
    for (int i =0; i< 16; i++)
        for (int j =0; j < 16; j++)
        {
            Board::GridEntry ge = bd.getContentsOf(i, j);
            if (ge == Board::player)
            {
                //Add Peach
                avatars.push_back (new PlayerAvatar (IID_PEACH,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,1, this));
                //Add Yoshi
                avatars.push_back (new PlayerAvatar (IID_YOSHI,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,2, this));
                //Peach & Yoshi start on a Blue Square
                actors.push_back (new CoinSquare (IID_BLUE_COIN_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
            }
        }
    //actors.push_back (new Vortex (IID_VORTEX, 0, 0, this, 0, 0, 1, 0));
    for (int i =0; i< 16; i++)
    {
        for (int j =0; j <16; j++)
        {
        Board::GridEntry ge = bd.getContentsOf(i, j);
        switch (ge)
        {
            case Board::empty:
                break;
            case Board::boo:
                actors.push_back(new Boo(IID_BOO,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                actors.push_back (new CoinSquare (IID_BLUE_COIN_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
            case Board::bowser:
                actors.push_back(new Bowser(IID_BOWSER,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                actors.push_back (new CoinSquare (IID_BLUE_COIN_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
            case Board::player:
                break;
            case Board::red_coin_square:
                actors.push_back (new CoinSquare (IID_RED_COIN_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
            case Board::blue_coin_square:
                //instead of getLandedOn being sent to CoinSquare, just do the coin addition/subtraction within PlayerAvatar
                actors.push_back (new CoinSquare (IID_BLUE_COIN_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
            case Board::up_dir_square:
                actors.push_back (new DirectionalSquare (IID_DIR_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1], Actor::up,1,1,Actor::up));
                break;
            case Board::down_dir_square:
                actors.push_back (new DirectionalSquare (IID_DIR_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1], Actor::down,1,1,Actor::down));
                break;
            case Board::left_dir_square:
                actors.push_back (new DirectionalSquare (IID_DIR_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1], Actor::left,1,1,Actor::left));
                break;
            case Board::right_dir_square:
                actors.push_back (new DirectionalSquare (IID_DIR_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1], Actor::right,1,1,Actor::right));
                break;
            case Board::event_square:
                actors.push_back (new EventSquare (IID_EVENT_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));

                break;
            case Board::bank_square:
                actors.push_back (new BankSquare (IID_BANK_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
            case Board::star_square:
                actors.push_back (new StarSquare (IID_STAR_SQUARE,SPRITE_WIDTH* i,SPRITE_HEIGHT * j,this,avatars[0],avatars[1]));
                break;
        }
        }
    }
        
        //should be 99
        startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
        return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::peachWon()
{
    if (avatars[0]->getStars() > avatars[1]->getStars())
        return true;
    else if (avatars[0]->getStars() < avatars[1]->getStars())
             return false;
    else if (avatars[0]->getStars() == avatars[1]->getStars())
    {
        if (avatars[0]->getCoins() > avatars[1]->getCoins())
            return true;
        else if (avatars[0]->getCoins() < avatars[1]->getCoins())
            return false;
    }
    //tie
    int rand = randInt(1, 2);
    if (rand == 1)
        return true;
    
    return false;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if (peachWon())
            setFinalScore(avatars[0]->getStars(), avatars[0]->getCoins());
        else
            setFinalScore(avatars[1]->getStars(), avatars[1]->getCoins());
        if (peachWon())
            return GWSTATUS_PEACH_WON;
        else
            return GWSTATUS_YOSHI_WON;
    }

    for (int i =0; i< actors.size(); i++)
    {
        actors[i]->doSomething();
    }
    for (int i =0; i< avatars.size(); i++)
    {
        avatars[i]->doSomething();
    }
    
    //checks to remove vortex
    vector<Actor*> :: iterator it = actors.begin();
    for (int i =0; i< actors.size(); i++)
    {
        if (actors[i]->canBeShot() && actors[i]->getActive() == false)
        {
            delete actors[i];
            actors.erase(it);
        }
        it++;
    }

    //Game Statistics Text
    
    //For Vortex
    string p1VOR = "";
    string p2VOR = "";
    if (avatars[0]->getHasVortex() == true)
        p1VOR = " VOR";
    else
        p1VOR = "";
    if (avatars[1]->getHasVortex() == true)
        p2VOR = " VOR";
    else
        p2VOR = "";
    
    string gameStatText =
    "P1 Roll: " + to_string(avatars[0]->getRoll()) + " Stars: " + to_string(avatars[0]->getStars()) + " $$: " + to_string(avatars[0]->getCoins()) + p1VOR +
    " | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getBankTotal())+ " |" +
    " P2 Roll: " + to_string(avatars[1]->getRoll()) + " Stars: " + to_string(avatars[1]->getStars()) + " $$: " + to_string(avatars[1]->getCoins()) + p2VOR
    ;
    setGameStatText(gameStatText);

    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (actors.size() != 0)
    {
        delete actors [actors.size()-1];
        actors.pop_back();
    }
    while (avatars.size() != 0)
    {
        delete avatars [avatars.size()-1];
        avatars.pop_back();
    }
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::deleteSquareAt(int x, int y)
{
    vector<Actor*> :: iterator it = actors.begin();
    for (int i =0; i< actors.size(); i++)
    {
        if ((*it)->canLandOn() == true && (*it)->getX() == x &&  (*it)->getY()  == y )
        {
            delete actors[i];
            actors.erase(it);
            return;
        }
        it++;
    }
}
void StudentWorld::insertDroppingSquareAt(int x, int y)
{
    actors.push_back (new DroppingSquare (IID_DROPPING_SQUARE,x,y,this,avatars[0],avatars[1]));
}

void StudentWorld :: createVortex(int x, int y, int direction)
{
    actors.push_back (new Vortex (IID_VORTEX, x , y, this, 0, 0, 1, direction));
}

bool StudentWorld::checkVortexOverlap()
{
    vector <Actor*> :: iterator itVortex = actors.begin();
    for (int i =0; i< actors.size(); i++)
    {
        if ((*itVortex)->canBeShot() == true)
        {
            break;
        }
            itVortex++;
    }
    vector <Actor*> :: iterator itBaddie = actors.begin();
    for (int i =0; i<actors.size(); i++)
    {
        if ((*itBaddie)->canHurt() == true)
        {
            //if overlapping
            if ((*itBaddie)-> getX()/SPRITE_WIDTH == (*itVortex)->getX()/SPRITE_WIDTH && (*itBaddie)->getY()/SPRITE_HEIGHT == (*itVortex)->getY()/SPRITE_HEIGHT)
            {
                (*itBaddie)->teleport();
                (*itBaddie)->setBeenImpacted(true);
                (*itVortex)->setActive(false);
                
                playSound(SOUND_HIT_BY_VORTEX);
                return true;
            }
        }
        itBaddie++;
    }
    return false;
}





