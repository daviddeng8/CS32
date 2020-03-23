#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

//idk if this next line is allowed
#include "Actor.h"

#include <string>
#include <cmath>
#include <list>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    //given functions 
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //initialization functions called by init to initialize items at the beginn
    void initPits();
    void initFood();
    void initDirt();
    
    //adds items to the game
    void addItems();
    void addFungus();
    void addGoodies();
    void addObject(Actor* object);
    
    //check for overlap
    bool overlap(Actor* first, Actor* second);
    bool movementOverlap(Actor* bact, Actor* dirt, int steps);
    
    //functions for bacteria to function properly
    void turnTowardsFood(Bacteria* bact);
    void turnEColi(Bacteria* bact);
    bool checkBlockedMovement(Bacteria* bact);
    bool consumeFood(Bacteria* bact);
    bool checkDamageSocrates(Bacteria* bact);
    bool socratesCloseEnough(Bacteria* bact, double distance);
    double distanceToSocrates(Actor* actor);
    double directionToSocrates(Bacteria* bact);
    bool ecoliMove(Bacteria* bact);
    
    //miscellaneous functions
    bool insideDish(Actor* actor, int steps);
    bool pickUpGoodies(Item* it);
    bool damageOverlap(Projectile* proj);
    
    //functions that help keep track of when the game should end
    void bacteriaDied() {m_nBacteria--;};
    void bacteriaBorn() {m_nBacteria++;}
    void pitDied() {m_nPits--;}


private:
    //Socrates pointer
    Socrates* m_socrates;
    std::vector<Actor*> objects;
    
    int m_nBacteria;
    int m_nPits;
    
    //returns if two points are within distance of each other
    bool appropriateDistance(double firstX, double firstY, double secondX, double secondY, double distance) {
        return sqrt(pow(firstX - secondX, 2) + pow(firstY - secondY, 2)) <= distance;
    }
    
    //returns the distance of two points to each other
    double distanceBetween(double firstX, double firstY, double secondX, double secondY) {
        return sqrt(pow(firstX - secondX, 2) + pow(firstY - secondY, 2));
    }
    
};

#endif // STUDENTWORLD_H_
