#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor Class

// Actor class constructor
Actor::Actor(int image_id, int x, int y, GraphObject::Direction dir, double size, int depth, StudentWorld* s_world, bool visible)
: GraphObject(image_id, x, y, dir, size, depth), m_world(s_world)
{
    setVisible(visible);
}

bool Actor::isAlive() const
{
    return state;
}

void Actor::setDead()
{
    state = false;
}

bool Actor::annoy(unsigned int amt)
{
    return true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

// Actor class destructor
Actor::~Actor()
{
    setVisible(false);
}


// Dirt Class

// Dirt class constructor
Dirt::Dirt(StudentWorld* s_world, int x, int y) : Actor(IID_DIRT, x, y, right, 0.25, 3, s_world, true)
{}

// Dirt class destructor
Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

// Agent Class
Agent::Agent(int image_id, int x, int y, Direction dir, double size, int depth, StudentWorld* s_world, bool visibility, int hp)
: Actor(image_id, x, y, dir, size, depth, s_world, true), m_hp(hp)
{
    
}
// Agent

unsigned int Agent::getHitPoints() const{
    return m_hp;
}

bool Agent::annoy(unsigned int amount){
    m_hp = m_hp - amount;
    return true;
}

// FrackMan Class

FrackMan::FrackMan(StudentWorld* s_world) : Agent(IID_PLAYER, 30, 60, right, 1.0, 0, s_world, true, 10), water_units(5), sonar_charge(1), gold_nuggets(0)
{
    
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
    if (!isAlive()){
        return;
    }
    
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (getWorld()->getObject(getX() + i, getY() + j) != nullptr
                && getWorld()->getObject(getX() + i, getY() + j)->getID() == IID_DIRT){
                getWorld()->clearDirt(getX(), getY());
                getWorld()->playSound(SOUND_DIG);
            }
        }
    }
    
    int key;
    if (getWorld()->getKey(key)){
        switch(key) {
            case KEY_PRESS_DOWN:
                if (getWorld()->canActorMoveTo(this, getX(), getY()-1) && getDirection() == down)
                { // if it's possible to move down
                    moveTo(getX(), getY()-1);
                }
                setDirection(down);
                break;
            case KEY_PRESS_UP:
                if (getWorld()->canActorMoveTo(this, getX(), getY()+1) && getDirection() == up)
                { // if it's possible to move up
                    moveTo(getX(), getY()+1);
                }
                setDirection(up);
                break;
            case KEY_PRESS_LEFT:
                if (getWorld()->canActorMoveTo(this, getX()-1, getY()) && getDirection() == left)
                { // if it's possible to move left
            
                    moveTo(getX()-1, getY());
                }
                setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (getWorld()->canActorMoveTo(this, getX()+1, getY()) && getDirection() == right)
                { // if it's possible to move right
                    moveTo(getX()+1, getY());
                }
                setDirection(right);
                break;
            case KEY_PRESS_ESCAPE:
                while (getHitPoints() > 0){
                    annoy(1);
                }
                break;
            case KEY_PRESS_SPACE:
                if (getWaterUnits() > 0){
                    if (canFireSquirt(getDirection())){
                        Squirt* sq = nullptr;
                        if (getDirection() == up){
                            sq = new Squirt(getWorld(), getX(), getY() + 4, up);
                        }
                        else if (getDirection() == down){
                            sq = new Squirt(getWorld(), getX(), getY() - 4, down);
                        }
                        else if (getDirection() == right){
                            sq = new Squirt(getWorld(), getX() + 4, getY(), right);
                        }
                        else{
                            sq = new Squirt(getWorld(), getX() - 4, getY(), left);
                        }
                        if (sq != nullptr){
                            getWorld()->addActor(sq);
                            water_units--;
                        }
                    }
                    else{
                        water_units--;
                    }
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                }
                break;
            case KEY_PRESS_TAB:
                // drop a Gold Nugget at current location, reduce Gold Count, lifetime is 100 ticks,
                // only pickupable by protestor
                ;
            case 'Z':
                ;
            case 'z':
                getWorld()->playSound(SOUND_SONAR);
                if (sonar_charge > 0){
                    getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
                    sonar_charge--;
                }
                break;
                
        }
    }
}

unsigned int FrackMan::getWaterUnits()
{
    return water_units;
}

unsigned int FrackMan::getGoldNuggets()
{
    return gold_nuggets;
}

unsigned int FrackMan::getSonarCharge()
{
    return sonar_charge;
}

void FrackMan::addGold(){
    gold_nuggets++;
}

void FrackMan::addSonar(){
    sonar_charge++;
    sonar_charge++;
}

bool FrackMan::canFireSquirt(Direction dir){
    if (getDirection() == up){
        for (int i = 0; i < 4; i++){
            if (getWorld()->getObject(getX(), getY() + i + 4) != nullptr){
                if (getWorld()->getObject(getX(), getY() + i + 4)->getID() == IID_DIRT || getWorld()->getObject(getX(), getY() + i)->getID() == IID_BOULDER){
                    return false;
                }
            }
        }
        return true;
    }
    else if (getDirection() == down){
        for (int i = 0; i < 4; i++){
            if (getWorld()->getObject(getX(), getY() - i) != nullptr){
                if (getWorld()->getObject(getX(), getY() - i)->getID() == IID_DIRT || getWorld()->getObject(getX(), getY() + i)->getID() == IID_BOULDER){
                    return false;
                }
            }
        }
        return true;
        
    }
    else if (getDirection() == right){
        for (int i = 0; i < 4; i++){
            if (getWorld()->getObject(getX() + i + 4, getY()) != nullptr){
                if (getWorld()->getObject(getX() + i + 4, getY())->getID() == IID_DIRT || getWorld()->getObject(getX(), getY() + i)->getID() == IID_BOULDER){
                    return false;
                }
            }
        }
        return true;
    }
    else{
        for (int i = 0; i < 4; i++){
            if (getWorld()->getObject(getX() - i, getY()) != nullptr){
                if (getWorld()->getObject(getX() - i, getY())->getID() == IID_DIRT || getWorld()->getObject(getX(), getY() + i)->getID() == IID_BOULDER){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

// boulder class

Boulder::Boulder(StudentWorld* s_world, int x, int y) : Actor(IID_BOULDER, x, y, down, 1.0, 1, s_world, true)
{}

void Boulder::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    
    // check the dirt below it
    if (!fallingState){
        if (getWorld()->getObject(getX() + 0, getY() - 1) == nullptr)
            if (getWorld()->getObject(getX() + 1, getY() - 1) == nullptr)
                if (getWorld()->getObject(getX() + 2, getY() - 1) == nullptr)
                    if (getWorld()->getObject(getX() + 3, getY() - 1) == nullptr)
                        waitingState = true;
    }
    
    if (waitingState)
    { // enter waiting state
        tick_tracker++; // keep track of # of ticks
        if (tick_tracker == 30){
            fallingState = true;
            waitingState = false;
        }
    }
    
    if (fallingState){
        getWorld()->playSound(SOUND_FALLING_ROCK);
        moveTo(getX(), getY()-1);
        for (int i = 0; i < 4; i++){
            if (getWorld()->getObject(getX()+i, getY()-1) != nullptr) // check if there is something below the boulder
            {
                setDead();
                setVisible(false);
            }
        }
        // add code for annoyance and loss of life, page 33
    }
}

// squirt class implementation

Squirt::Squirt(StudentWorld*s_world, int x, int y, Direction dir)
: Actor(IID_WATER_SPURT, x, y, dir, 1.0, 1, s_world, true), m_dir(dir), m_initialX(x), m_initialY(y)
{}

void Squirt::doSomething(){
    
    for (int i = -1; i < 4; i++){
        for (int j = -1; j < 4; j++){
            // scan below the squirt
            if (getWorld()->getObject(getX() + i, getY() + j) != nullptr &&
                (getWorld()->getObject(getX() + i, getY() + j)->getID() == IID_PROTESTER ||
                 getWorld()->getObject(getX() + i, getY() + j)->getID() == IID_HARD_CORE_PROTESTER)){
                    getWorld()->getObject(getX() + i, getY() + j)->annoy(2);
                    setDead();
                    return;
                }
        }
    }
    
    
    if (getX() == m_initialX + 4 || getY() == m_initialY + 4 || getX() == m_initialX - 4 || getY() == m_initialY - 4){
        setDead();
        return;
    }
    if (m_dir == right){
        if (getWorld()->getObject(getX() + 4, getY()) == nullptr ||(
            getWorld()->getObject(getX() + 4, getY())->getID() != IID_BOULDER &&
            getWorld()->getObject(getX() + 4, getY())->getID() != IID_DIRT))
        {
            moveTo(getX() + 1, getY());
        }
        else{
            setDead();
        }
    }
    else if (m_dir == left){
        if (getWorld()->getObject(getX() - 1, getY()) == nullptr ||
            (getWorld()->getObject(getX() - 1, getY())->getID() != IID_BOULDER &&
            getWorld()->getObject(getX() - 1, getY())->getID() != IID_DIRT))
        {
            moveTo(getX() - 1, getY());
        }
        else{
            setDead();
        }
    }
    else if (m_dir == up){
        if (getWorld()->getObject(getX(), getY() + 4) == nullptr ||
            (getWorld()->getObject(getX(), getY() + 4)->getID() != IID_BOULDER &&
             getWorld()->getObject(getX(), getY() + 4)->getID() != IID_DIRT))
        {
            moveTo(getX(), getY() + 1);
        }
        else{
            setDead();
        }
    }
    else if (m_dir == down){
        if (getWorld()->getObject(getX(), getY() - 1) == nullptr ||
            (getWorld()->getObject(getX(), getY() - 1)->getID() != IID_BOULDER &&
             getWorld()->getObject(getX(), getY() - 1)->getID() != IID_DIRT))
        {
            moveTo(getX(), getY() - 1);
        }
        else{
            setDead();
        }
    }
}

// ActivatingObject class
ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
                 int soundToPlay, bool activateOnPlayer,
                 bool activateOnProtester, bool initallyActive, bool visible)
: Actor(imageID, startX, startY, right, 1.0, 2, world, visible)
{}

// Barrel of Oil Class

OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
: ActivatingObject(world, startX, startY, IID_BARREL, SOUND_GOT_GOODIE, true, false, false, false)
{
    
}

void OilBarrel::doSomething()
{
    if(!isAlive())
        return;
    
    if (!isVisible()){
        if (getWorld()->isNearFrackMan(this, 5)){
            setVisible(true);
            return;
        }
    }

    if (getWorld()->isNearFrackMan(this, 4)){
        getWorld()->playSound(SOUND_FOUND_OIL);
        setDead();
        getWorld()->increaseScore(1000);
        
    }

}

// Gold Nugget Class

GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temporary, bool activateOnPlayer)
: ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, activateOnPlayer, false, true, false), m_temporary(temporary)
{

}

bool GoldNugget::isTemporary() const
{
    return m_temporary;
}

void GoldNugget::doSomething()
{
    if(!isAlive())
        return;
    
    if(!isVisible()){
        if (getWorld()->isNearFrackMan(this, 5)){
            setVisible(true);
            return;
        }
    }
    
    if (getWorld()->isNearFrackMan(this, 4)){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
        getWorld()->increaseScore(10);
        getWorld()->getFrackMan()->addGold();
    }
}


// Sonar Kit Class

SonarKit::SonarKit(StudentWorld* world, int startX, int startY, int ticks)
: ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, true, true), m_ticks(ticks)
{
}

void SonarKit::doSomething()
{
    if (!isAlive())
        return;
    
    if (getWorld()->isNearFrackMan(this, 4)){
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
        getWorld()->increaseScore(75);
        getWorld()->getFrackMan()->addSonar();
    }
    else{
        m_ticks--;
    }
    
    if (m_ticks == 0){
        setDead();
    }
}
