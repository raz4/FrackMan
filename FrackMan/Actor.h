#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// base, Star, and player class declarations
// as well as constants required by these classes

class Actor : public GraphObject
{
public:
    Actor(int image_id, int x, int y, Direction dir, double size, int depth, StudentWorld* s_world, bool visible);
    
    ~Actor();
    
    // Get this Actor's world
    StudentWorld* getWorld() const;
    
    // Action to perform each tick
    virtual void doSomething() = 0;
    
    // given interfaces //
    
    // Is this actor alive?
    bool isAlive() const;
    
    // Mark this actor as dead.
    void setDead();
    
    // Can other actors pass through this actor?
    // virtual bool canActorsPassThroughMe() const;
    
    // Annoy this actor.
    virtual bool annoy(unsigned int amt);
    
    /*
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const;
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;
    
    // Does this actor hunt the FrackMan?
    virtual bool huntsFrackMan() const;
    
    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const;
    
    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);
    */
private:
    StudentWorld* m_world;
    bool state = true; // alive
};

// Dirt
class Dirt : public Actor
{
public:
    Dirt(StudentWorld* s_world, int x, int y);
    
    ~Dirt();
    
    virtual void doSomething();
};

// Agents

class Agent : public Actor
{
public:
    Agent(int image_id, int x, int y, Direction dir, double size, int depth, StudentWorld* s_world, bool visibility, int hp);
    
    // Pick up a gold nugget.
    virtual void addGold() = 0;
    
    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;
    
    // always returns true?
    virtual bool annoy(unsigned int amount);
    
    //virtual bool canPickThingsUp() const;
    
private:
    int m_hp;
    
};

class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* s_world);
    
    ~FrackMan();
    
    virtual void doSomething();
    
    unsigned int getWaterUnits();
    
    unsigned int getGoldNuggets();
    
    unsigned int getSonarCharge();
    
    virtual void addGold();
    
    // Pick up a sonar kit.
    void addSonar();
    
    bool canFireSquirt(Direction dir);
    
    // given interfaces //
    /*

    virtual bool canDigThroughDirt() const;
    
    // Pick up water.
    void addWater();
   */
    
private:
    int water_units;
    int sonar_charge;
    int gold_nuggets;
};

class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
              unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsFrackMan() const;
    
    // Set state to having gien up protest
    void setMustLeaveOilField();
    
    // Set number of ticks until next move
    void setTicksToNextMove();
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual void addGold();
};

class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual void addGold();
};

// boulder
class Boulder : public Actor
{
public:
    Boulder(StudentWorld* s_world, int x, int y);
    
    virtual void doSomething();
    
private:
    int tick_tracker = 1;
    bool waitingState = false;
    bool fallingState = false;
};

// squirt
class Squirt : public Actor
{
public:
    Squirt(StudentWorld* s_world, int x, int y, Direction dir);
    
    virtual void doSomething();

private:
    Direction m_dir;
    int m_initialX;
    int m_initialY;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
                     int soundToPlay, bool activateOnPlayer,
                     bool activateOnProtester, bool initallyActive, bool visible);
};

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
};

class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temporary, bool activateOnPlayer);
    virtual void doSomething();
    bool isTemporary() const;

private:
    bool m_temporary;
};

class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* world, int startX, int startY, int ticks);
    virtual void doSomething();
    
private:
    int m_ticks;
};

#endif // ACTOR_H_
