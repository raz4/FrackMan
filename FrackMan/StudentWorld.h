#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class FrackMan;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    
    ~StudentWorld();

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    // return a pointer to the object at the given coordinates
    Actor* getObject(int x, int y) const;
    
    // return a pointer to the object with a specific ID and coordinates
    Actor* getObject(int x, int y, int ID) const;

    // return FrackMan pointer
    FrackMan* getFrackMan() const;
    
    // return number of barrels, also used for checking if level is completed
    int numberOfBarrels() const;
    
    bool getEmptyValidCoordinates(int& x, int& y);
    
    // remove all dead actors
    void removeDeadGameObjects();
    
    // update the score/lives/level at screen top
    void updateDisplayText();
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Clear a 4x4 region of dirt.
    void clearDirt(int x, int y);
    
    // Can actor move to x,y?
    bool canActorMoveTo(Actor* a, int x, int y) const;
    
    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);
    
    // Return whether the Actor a is within radius of FrackMan.
    bool isNearFrackMan(Actor* a, int radius) const;
    
    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    //int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    
    // If the FrackMan is within radius of a, return a pointer to the
    // FrackMan, otherwise null.
    //Actor* findNearbyFrackMan(Actor* a, int radius) const;
    
    // If at least one actor that can pick things up is within radius of a,
    // return a pointer to one of them, otherwise null.
    //Actor* findNearbyPickerUpper(Actor* a, int radius) const;
    
    // Annoy the FrackMan.
    //void annoyFrackMan();
    
    // Give FrackMan some sonar charges.
    //void giveFrackManSonar();
    
    // Give FrackMan some water.
    //void giveFrackManWater();
    
    // Is the Actor a facing toward the FrackMan?
    //bool facingTowardFrackMan(Actor* a) const;
    
    // If the Actor a has a clear line of sight to the FrackMan, return
    // the direction to the FrackMan, otherwise GraphObject::none.
    //GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;
    
    // Determine the direction of the first move a quitting protester
    // makes to leave the oil field.
    //GraphObject::Direction determineFirstMoveToExit(int x, int y);
    
    // Determine the direction of the first move a hardcore protester
    // makes to approach the FrackMan.
    //GraphObject::Direction determineFirstMoveToFrackMan(int x, int y);

private:
    std::list<Actor*> AssetList;
    
    FrackMan* m_player;
    
    bool playerDiedDuringThisTick = false;
    
    int barrels_remaining;
};

#endif // STUDENTWORLD_H_
