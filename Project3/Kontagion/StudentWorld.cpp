#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

const double PI = 4 * atan(1);

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

//initializes the actors at the start of the game 
int StudentWorld::init()
{
    m_nPits = getLevel();
    m_nBacteria = m_nPits * 10;
    
    m_socrates = new Socrates(this);
    
    initPits();
    initFood();
    initDirt();
   
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::initPits() {
    int x;
    int y;
    for (int i = 0; i < getLevel();) {
        x = randInt(0, 256);
        y = randInt(0, 256);
        
        //ensure that the newx and newy values are within the required radius
        while (!appropriateDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2, 120)) {
            x = randInt(0, 256);
            y = randInt(0, 256);
        }
        bool valid = true;
        for (int i = 0; i < objects.size(); i++) {
            //if it overlaps something that can't be overlapped, then don't add the object and re-enter the loop within incrementing i
            if (objects[i]->canBeOverlappedCreating() == false) {
                if (appropriateDistance(x, y, objects[i]->getX(), objects[i]->getY(), SPRITE_WIDTH)) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            addObject(new Pit(this, x, y));
            i++;
        }
    }
}

void StudentWorld::initFood() {
    int x;
    int y;
    //min(5 * getLevel(), 25)
    for (int i = 0; i < min(5 * getLevel(), 25);) {
        x = randInt(0, 256);
        y = randInt(0, 256);
        
        //ensure that the newx and newy values are within the required radius
        while (!appropriateDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2, 120)) {
            x = randInt(0, 256);
            y = randInt(0, 256);
        }
        bool valid = true;
        for (int i = 0; i < objects.size(); i++) {
            //if it overlaps something that can't be overlapped, then don't add the object and re-enter the loop within incrementing i
            if (objects[i]->canBeOverlappedCreating() == false) {
                if (appropriateDistance(x, y, objects[i]->getX(), objects[i]->getY(), SPRITE_WIDTH)) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            addObject(new Food(this, x, y));
            i++;
        }
    }
}

void StudentWorld::initDirt() {
    for (int i = 0; i < std::max(180 - 20*getLevel(), 20);) {
           int x = randInt(0, 256);
           int y = randInt(0, 256);
        
           //ensure that the newx and newy values are within the required radius
           while (!appropriateDistance(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2, 120)) {
               x = randInt(0, 256);
               y = randInt(0, 256);
           }
           bool valid = true;
           for (int i = 0; i < objects.size(); i++) {
               //if it overlaps something that can't be overlapped, then don't add the object and re-enter the loop within incrementing i
               if (objects[i]->canBeOverlappedCreating() == false) {
                   if (appropriateDistance(x, y, objects[i]->getX(), objects[i]->getY(), SPRITE_WIDTH)) {
                       valid = false;
                       break;
                   }
               }
           }
           if (valid) {
               addObject(new Dirt(this, x, y));
               i++;
           }
       }
}

int StudentWorld::move()
{
    //make all actors do their action
    m_socrates->doSomething();
    
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->doSomething();
        m_socrates->checkAlive();
        if (m_socrates->alive() == false) {
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (m_nBacteria == 0 && m_nPits == 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    //deleting dead actors
    for (int i = 0; i < objects.size();) {
        if (objects[i]->alive() == false) {
            delete objects[i];
            objects.erase(objects.begin() + i);
            continue;
        }
        i++;
    }
        
    //introducing new items to the game
    addItems();
    
    //updating the score board
    ostringstream oss;
    oss.setf(ios::fixed);
    /*
    oss << "Score:  " << setw(6) << setfill('0') << getScore() << "  Level:  " << getLevel() << "  Lives:  " << getLives() << "  Health:  " << m_socrates->gethp() << "  Sprays:  " << m_socrates->getSprays() << "  Flames:  " << m_socrates->getFlameCharges();*/
    
    oss << "Score:  ";
    int numfill = 6;
    int displayScore = getScore();
    if (getScore() < 0) {
        numfill--;
        oss << "-";
        displayScore *= -1;
    }
    oss << setw(numfill) << setfill('0') << displayScore << "  Level:  " << getLevel() << "  Lives:  " << getLives() << "  Health:  " << m_socrates->gethp() << "  Sprays:  " << m_socrates->getSprays() << "  Flames:  " << m_socrates->getFlameCharges();
    
    string s = oss.str();
    
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    while (objects.size() != 0) {
        delete objects[0];
        objects.erase(objects.begin());
    }
    
    delete m_socrates;
    m_socrates = nullptr;
}

bool StudentWorld::pickUpGoodies(Item* it) {
    if (m_socrates->canConsumeItems() && overlap(it, m_socrates)) {
        increaseScore(it->points());
        it->setDead();
        playSound(it->getSound());
        it->performAction(m_socrates);
        return true;
    }
    
    return false;
}

//overlap function for two actors
bool StudentWorld::overlap(Actor* first, Actor* second) {
    return appropriateDistance(first->getX(), first->getY(), second->getX(), second->getY(), SPRITE_WIDTH);
}

//checks movement overlap between an actor and dirt
bool StudentWorld::movementOverlap(Actor* bact, Actor* dirt, int steps) {
    //redo this part with the function in the GraphObject class that does this for you
    double dx;
    double dy;
    bact->getPositionInThisDirection(bact->getDirection(), steps, dx, dy);
    
    return appropriateDistance(dx, dy, dirt->getX(), dirt->getY(), SPRITE_WIDTH/2);
}

//checks if there's a dirt that can block the bacteria
bool StudentWorld::checkBlockedMovement(Bacteria* bact) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->canBlock() && movementOverlap(bact, objects[i], bact->getSteps())) {
            return true;
        }
    }
    return false;
}

//turns a bacteria towards the nearest food object
void StudentWorld::turnTowardsFood(Bacteria* bact) {
    double dist = -1;
    int loc = -1;
    
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->canBeHurtByBacteria()) {
            double cur = distanceBetween(bact->getX(), bact->getY(), objects[i]->getX(), objects[i]->getY());
            if (dist == -1 && loc == -1) {
                loc = i;
                dist = cur;
            }
            else if (cur < dist) {
                loc = i;
                dist = cur;
            }
        }
    }
    
    //if there's no food nearby, then pick a random direction
    if (dist == -1 && loc == -1) {
        bact->chooseRandomDirection();
        return;
    }
    
    //else, set to nearest food direction
    bact->setDirection(180 + (atan2(bact->getY() - objects[loc]->getY(), bact->getX() - objects[loc]->getX()) * 180 / PI));
    
    //if it can't move towards nearest food then choose random direction to turn towards
    if (checkBlockedMovement(bact)) {
        bact->chooseRandomDirection();
    }
    else {
        bact->resetMovementPlan();
    }
}

void StudentWorld::addItems() {
    addFungus();
    addGoodies();
}

//sometimes introduces a new fungus to the world
void StudentWorld::addFungus() {
    int chanceFungus = max(510 - getLevel() * 10, 200);
    if (randInt(0, chanceFungus) == 0) {
        int rand = randInt(0, 71);
        objects.push_back(new FungusItem(this, 128 - 128 * cos(rand * 5 * 1.0 / 360 * 2 * PI), 128 + 128 * sin(rand * 5 * 1.0 / 360 * 2 * PI))); //MAKE A NEW FUNCTION FOR THIS
    }
}

//sometimes introduces new goodies to the world
void StudentWorld::addGoodies() {
    int chanceGoodie = max(510 - getLevel() * 10, 250);
    if (randInt(0, chanceGoodie) == 0) {
        //add goodie, 60% restore health, 30% flamethrower, 10% extra life
        int whichgoodie = randInt(0, 9);
        int rand = randInt(0, 71);
        if (whichgoodie < 6) {
            addObject(new RestoreHealthItem(this, 128 - 128 * cos(rand * 5 * 1.0 / 360 * 2 * PI), 128 + 128 * sin(rand * 5 * 1.0 / 360 * 2 * PI)));
        } else if (whichgoodie < 9) {
            addObject(new FlamethrowerItem(this, 128 - 128 * cos(rand * 5 * 1.0 / 360 * 2 * PI), 128 + 128 * sin(rand * 5 * 1.0 / 360 * 2 * PI)));
        } else {
            addObject(new ExtraLifeItem(this, 128 - 128 * cos(rand * 5 * 1.0 / 360 * 2 * PI), 128 + 128 * sin(rand * 5 * 1.0 / 360 * 2 * PI)));
        }
    }
}

void StudentWorld::addObject(Actor* object) {
    objects.push_back(object);
}

bool StudentWorld::consumeFood(Bacteria* bact) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->isEdible() && overlap(bact, objects[i])) {
            objects[i]->setDead();
            bact->incrementFoodCount();
            return true;
        }
    }
    return false;
}

//checks if moving steps pixels in current direction would make an actor go out of the dish or not
bool StudentWorld::insideDish(Actor* actor, int steps) {
    double dx;
    double dy;
    actor->getPositionInThisDirection(actor->getDirection(), steps, dx, dy);
    return distanceBetween(dx, dy, VIEW_WIDTH/2, VIEW_HEIGHT/2) <= VIEW_RADIUS;
}

bool StudentWorld::checkDamageSocrates(Bacteria* bact) {
    if (overlap(bact, m_socrates)) {
        m_socrates->takeDamage(bact->getDamage());
        return true;
    }
    return false;
}

bool StudentWorld::socratesCloseEnough(Bacteria* bact, double distance) {
                
    if (distanceBetween(bact->getX(), bact->getY(), m_socrates->getX(), m_socrates->getY()) <= distance) {
        bact->setDirection(directionToSocrates(bact));
        if (!checkBlockedMovement(bact)) {
            if (insideDish(bact, bact->getSteps())) {
                bact->moveAngle(bact->getDirection(), 3);
            }
        }
        return true;
    }
    return false;
}

double StudentWorld::distanceToSocrates(Actor* actor) {
    return distanceBetween(actor->getX(), actor->getY(), m_socrates->getX(), m_socrates->getY());
}

//does the logic for ecoli moving in its current direction
bool StudentWorld::ecoliMove(Bacteria *bact) {
    if (checkBlockedMovement(bact) || !insideDish(bact, bact->getSteps())) {
        return false;
    }
    bact->moveAngle(bact->getDirection(), bact->getSteps());
    return true;
}

double StudentWorld::directionToSocrates(Bacteria* bact) {
    return (180 + (atan2(bact->getY() - m_socrates->getY(), bact->getX() - m_socrates->getX()) * 180 / PI));
}

//checks if a projectile is overlapping anything that can be destroyed/hurt by it
bool StudentWorld::damageOverlap(Projectile* proj) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->damageableByProjectiles() && overlap(proj, objects[i])) {
            if (objects[i]->hashp()) {
                objects[i]->takeDamage(proj->getDamage());
            }
            objects[i]->hitByProjectile();
            return true;
        }
    }
    return false;
}

