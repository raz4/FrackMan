#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
   
}

int StudentWorld::init()
{
    /*
     A. Initialize the data structures used to keep track of your game’s virtual world
     B. Construct a new oil field that meets the requirements stated in the section below
     (filled with Dirt, Barrels of oil, Boulders, Gold Nuggets, etc.)
     C. Allocate and insert a valid player object into the game world at the proper
     location
    */
    
    int current_level = getLevel();
    int x_coord;
    int y_coord;
    
    // dirt
    for (int row = 0; row < 4 ; row++){
        for (int col = 0; col < VIEW_WIDTH; col++){
            Dirt* d = new Dirt(this, col, row);
            addActor(d);
        }
    }
    for (int row = 4; row < 60 ; row++){
        for (int col = 0; col < 30; col++){
            Dirt* d = new Dirt(this, col, row);
            addActor(d);
        }
        for (int col = 34; col < 64; col++){
            Dirt* d = new Dirt(this, col, row);
            addActor(d);
        }
    }
    
    // load Barrels of oil
    barrels_remaining = min(2 + current_level, 20);
    for (int i = 0; i < barrels_remaining; i++){
        getEmptyValidCoordinates(x_coord, y_coord);
        OilBarrel* ob = new OilBarrel(this, x_coord, y_coord);
        addActor(ob);
    }
    
    // load boulder, page 32
    int boulder_count = min(current_level / 2 + 2, 6);
    
    for (int i = 0; i < boulder_count; i++){
        getEmptyValidCoordinates(x_coord, y_coord);
        // add the boulder to the list
        clearDirt(x_coord, y_coord);
        Boulder* b = new Boulder(this, x_coord, y_coord);
        addActor(b);
    }
    
    // load gold nuggets
    int gold_count = max(5 - current_level / 2, 2);
    for (int i = 0; i < gold_count; i++){
        getEmptyValidCoordinates(x_coord, y_coord);
        GoldNugget* gn = new GoldNugget(this, x_coord, y_coord, false, true);
        addActor(gn);
    }
    
    // load sonar_charges
    int ticks = max(100, 300-10*current_level);
    getEmptyValidCoordinates(x_coord, y_coord);
    SonarKit* sk = new SonarKit(this, x_coord, y_coord, ticks);
    addActor(sk);
    
    // add FrackMan
    FrackMan* fm = new FrackMan(this);
    
    //addActor(fm);
    m_player = fm;

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateDisplayText();

    // FrackMan does something
    m_player->doSomething();
    
    // doSomething function for each actor
    for (std::list<Actor*>::iterator it = AssetList.begin(); it != AssetList.end(); it++){
        Actor* gobj = *it;
        //it++ removes error if FrackMan is put into AssetList
        if (gobj->isAlive()){
            gobj->doSomething();
        }
    }
    
    // remove all "dead" actors
    removeDeadGameObjects();
    
    // when FrackMan dies...
    if (m_player->getHitPoints() <= 0){
        m_player->setDead();
        m_player->setVisible(false);
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    // If the player has collected all of the Barrels on the level, then // return the result that the player finished the level
    if (numberOfBarrels() <= 0){
        delete m_player;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

///////

// check if player died during this tick
/*bool StudentWorld::theplayerDiedDuringThisTick(){
    if (playerDiedDuringThisTick){
        playerDiedDuringThisTick = false;
        return true;
    }
    return false;
}*/

// generate cooridnates for initializating objects
bool StudentWorld::getEmptyValidCoordinates(int& x, int& y){
    bool valid = false;
    while (!valid){
        valid = true;
        
        x = rand() % 58;
        while ( x > 26 && x < 34){
            x = rand() % 58;
        }
        y = rand() % 56 + 1;
        
        // check coordinates
        std::list<Actor*>::iterator it = AssetList.begin();
        while (it != AssetList.end())
        {
            Actor* gobj = *it;
            if ( gobj->getID() != IID_DIRT ){
                int obj_x = gobj->getX();
                int obj_y = gobj->getY();
                
                double first = pow(x-obj_x, 2);
                double second = pow(y-obj_y, 2);
                if (sqrt(first + second) < 6){
                    valid = false;
                    break;
                }
            }
            it++;
        }
    }

    return true;

}

FrackMan* StudentWorld::getFrackMan() const
{
    return m_player;
}

int StudentWorld::numberOfBarrels() const
{
    return barrels_remaining;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius){
    int inverse_radius = radius - (radius*2);
    for (int i = inverse_radius; i < radius + 4; i++){
        for (int j = inverse_radius; j < radius + 4; j++){
            if (getObject(x + i, y + j, IID_GOLD) != nullptr)
            {
                getObject(x + i, y + j, IID_GOLD)->setVisible(true);
            }
            if (getObject(x + i, y + j, IID_BARREL) != nullptr)
            {
                getObject(x + i, y + j, IID_BARREL)->setVisible(true);
            }
        }
    }
}

// remove newly-dead actors after each tick
void StudentWorld::removeDeadGameObjects()
{
    for (std::list<Actor*>::iterator it = AssetList.begin(); it != AssetList.end(); )
    {
        Actor* gobj = *it;
        if (!gobj->isAlive()){
            if (gobj->getID() == IID_PLAYER){
                playerDiedDuringThisTick = true;
            }
            if (gobj->getID() == IID_BARREL){
                barrels_remaining = barrels_remaining - 1;
            }
            it++;
            AssetList.remove(gobj);
            delete gobj;
        }
        else{
            it++;
        }
    }
}

// update the score/lives/level at screen top
void StudentWorld::updateDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int hitPoints = m_player->getHitPoints();
    int squirts = m_player->getWaterUnits();
    int gold = m_player->getGoldNuggets();
    int sonar = m_player->getSonarCharge();
    int barrelsLeft = numberOfBarrels();
    
    int percentage = (hitPoints / 10) * 100;
    
    ostringstream scoreString;
    scoreString << "Scr: " << setw(6);
    scoreString.fill('0');
    scoreString << score;
    
    ostringstream levelString;
    levelString << "Lvl: " << setw(2);
    levelString.fill(' ');
    levelString << level;
    
    ostringstream livesString;
    livesString << "Lives: " << setw(1);
    livesString << lives;
    
    ostringstream hpString;
    hpString << "Hlth: " << setw(3);
    hpString.fill(' ');
    hpString << percentage << "%";
    
    ostringstream waterString;
    waterString << "Wtr: " << setw(2);
    waterString.fill(' ');
    waterString << squirts;
    
    ostringstream goldString;
    goldString << "Gld: " << setw(2);
    goldString.fill(' ');
    goldString << gold;
    
    ostringstream sonarString;
    sonarString << "Sonar: " << setw(2);
    sonarString.fill(' ');
    sonarString << sonar;
    
    ostringstream barrelString;
    barrelString << "Oil Left: " << setw(2);
    barrelString.fill(' ');
    barrelString << barrelsLeft;
    
    string s = scoreString.str() + "  " + levelString.str() + "  " + livesString.str() + "  "
            + hpString.str() + "  " + waterString.str() + "  " + goldString.str() + "  "
            + sonarString.str() + "  " + barrelString.str();
    
    setGameStatText(s);
    
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a){
    AssetList.push_back(a);
}

// Clear a 4x4 region of dirt.
void StudentWorld::clearDirt(int x, int y){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            Actor* gobj = getObject(x + i, y + j);
            if (gobj != nullptr && gobj->getID() == IID_DIRT){
                gobj->setDead();
                //AssetList.remove(gobj);
                //delete gobj;
            }
            
        }
    }
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const{
    if (x < 0 || y < 0 || y > 60)
        return false;
    
    for (int i = -3; i < 4; i++){
        for (int j = -3; j < 4; j++){
            /*if (getObject(x-i, y-j) != nullptr && getObject(x-i, y-j)->getID() != IID_DIRT && getObject(x-i, y-j)->getID() != IID_PLAYER){
                return false;
            }*/
            if (getObject(x-i, y-j) != nullptr && getObject(x-i, y-j)->getID() == IID_BOULDER){
                return false;
            }
        }
    }
    return true;
}

// check if near FrackMan
bool StudentWorld::isNearFrackMan(Actor* a, int radius) const
{
    for (int i = radius - 7; i < radius; i++){
        for (int j = radius - 7; j < radius; j++){
            if (getFrackMan()->getX() == a->getX() + i && getFrackMan()->getY() == a->getY() + j){
                return true;
                
            }
        }
    }
    return false;
}

Actor* StudentWorld::getObject(int x, int y) const
{
    list<Actor*>::const_iterator it = AssetList.begin();
    while(it != AssetList.end())
    {
        Actor* gobj = *it;
        if(gobj->getX() == x && gobj->getY() == y)
        {
            return gobj;
        }
        it++;
    }
    return nullptr;
}

Actor* StudentWorld::getObject(int x, int y, int ID) const
{
    list<Actor*>::const_iterator it = AssetList.begin();
    while(it != AssetList.end())
    {
        Actor* gobj = *it;
        if(gobj->getX() == x && gobj->getY() == y && gobj->getID() == ID)
        {
            return gobj;
        }
        it++;
    }
    return nullptr;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = AssetList.begin();
    while(it != AssetList.end()){
        Actor* gobj = *it;
        delete gobj;
        AssetList.erase(it);
        it++;
    }
}

StudentWorld::~StudentWorld(){
    list<Actor*>::iterator it = AssetList.begin();
    while(it != AssetList.end()){
        Actor* gobj = *it;
        delete gobj;
        AssetList.erase(it);
        it++;
    }
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
