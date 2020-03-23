#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

using namespace std;

const double PI = 4 * atan(1);


/*
       ACTOR FUNCTIONS
*/
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int depth) :GraphObject(imageID, startX, startY, startDirection, depth) {
    m_alive = true;
    m_world = world;
}

/*
      LIVINGBEING FUNCTIONS
*/
LivingBeing::LivingBeing(StudentWorld* world, int imageID, double startX, double startY, int hp, Direction startDirection) :Actor(world, imageID, startX, startY, startDirection, 0) {
    m_hp = hp;
}


void LivingBeing::sethp(int newhp) {
    m_hp = newhp;
}

void LivingBeing::takeDamage(int damage) {
    m_hp -= damage;
}


/*
       BINARY FUNCTIONS
*/
Binary::Binary(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection) :Actor(world, imageID, startX, startY, startDirection, 1) {
    
}

/*
        SOCRATES FUNCTIONS
 */
Socrates::Socrates(StudentWorld* world) :LivingBeing(world, IID_PLAYER, 0, VIEW_HEIGHT/2, 100, 0) //REDO THIS – USE THE GETPOSITION IN THIS DIRECTION FUNCTION IN GRAPHOBJECT.H
{
    sethp(100);
    m_sprays = 20;
    m_flamethrowercharges = 5;
}

void Socrates::doSomething() {
    
    if (!alive()) {
        return;
    }

    int ch;
    if (wp()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT:
                //to get to 90 – takes eighteen moves
                
                //0 degrees: 0, 128
                //270 degrees: 128, 256
                //180 degrees: 256, 128
                //90 degrees: 128, 0

                //to change the direction by 5 degrees
                setDirection(getDirection() - 5);
                moveTo(128 - 128 * cos((360 - getDirection()) * 1.0 / 360 * 2 * PI), 128 + 128 * sin((360 - getDirection()) * 1.0 / 360 * 2 * PI));
                break;
            case KEY_PRESS_RIGHT:
                setDirection(getDirection() + 5);
                moveTo(128 - 128 * cos((360 - getDirection()) * 1.0 / 360 * 2 * PI), 128 + 128 * sin((360 - getDirection()) * 1.0 / 360 * 2 * PI));
                break;
            case KEY_PRESS_SPACE:
                if (m_sprays > 0) {
                    double dx;
                    double dy;
                    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, dx, dy);
                    wp()->addObject(new Spray(wp(), dx, dy, getDirection()));
                    wp()->playSound(SOUND_PLAYER_SPRAY);
                    m_sprays--;
                }
                break;
            case KEY_PRESS_ENTER:
                if (m_flamethrowercharges > 0) {
                    double dx;
                    double dy;
                    for (int i = 0; i < 16; i++) {
                        getPositionInThisDirection(getDirection() + 22 * i, SPRITE_WIDTH, dx, dy);
                        wp()->addObject(new Flame(wp(), dx, dy, getDirection() + 22 * i));
                    }
                    wp()->playSound(SOUND_PLAYER_FIRE);
                    m_flamethrowercharges--;
                }
                break;
            default:
                break;
        }
    }
    else {
        //increment number of sprays if it's less than the max number
        if (m_sprays < 20) {
            m_sprays++;
        }
    }
}

void Socrates::addFlamethrowerCharges(int newFlamethrowerCharges) {
    m_flamethrowercharges += newFlamethrowerCharges;
}

void Socrates::checkAlive() {
    if (gethp() <= 0) {
        setDead();
        wp()->decLives();
    }
}

/*
       DIRT FUNCTIONS
*/
Dirt::Dirt(StudentWorld* world, double startX, double startY) :Binary(world, IID_DIRT, startX, startY, 0) {
    
}

/*
       FOOD FUNCTIONS
*/
Food::Food(StudentWorld* world, double startX, double startY) :Binary(world, IID_FOOD, startX, startY, 90) {
    
}

/*
       ITEM FUNCTIONS
*/
Item::Item(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int points) :Binary(world, imageID, startX, startY, startDirection) {
    
    m_lifetime = std::max(randInt(0, 300 - 10 * world->getLevel() - 1), 50);
    m_points = points;
    m_sound = SOUND_GOT_GOODIE;
}


void Item::doSomething() {
    if (!alive()) {
        return;
    }
    wp()->pickUpGoodies(this);
    lifetimeExpended();
}

void Item::lifetimeExpended() {
    m_lifetime = m_lifetime - 1;
    if (m_lifetime <= 0) {
        setDead();
    }
}

/*
       RESTOREHEALTHITEM FUNCTIONS
*/
RestoreHealthItem::RestoreHealthItem(StudentWorld* world, double startX, double startY) :Item(world, IID_RESTORE_HEALTH_GOODIE, startX, startY, 0, 250) {
    
}
    
void RestoreHealthItem::performAction(Socrates* object) {
    object->sethp(100);
}

/*
       FLAMETHROWERITEM FUNCTIONS
*/
FlamethrowerItem::FlamethrowerItem(StudentWorld* world, double startX, double startY) :Item(world, IID_FLAME_THROWER_GOODIE, startX, startY, 0, 300) {
    
}

void FlamethrowerItem::performAction(Socrates* object) {
    object->addFlamethrowerCharges(5);
}
    
/*
       EXTRALIFEITEM FUNCTIONS
*/
ExtraLifeItem::ExtraLifeItem(StudentWorld* world, double startX, double startY) :Item(world, IID_EXTRA_LIFE_GOODIE, startX, startY, 0, 500) {
    
}

void ExtraLifeItem::performAction(Socrates*) {
    wp()->incLives();
}

/*
       FUNGUSITEM FUNCTIONS
*/
FungusItem::FungusItem(StudentWorld* world, double startX, double startY) :Item(world, IID_FUNGUS, startX, startY, 0, -50) {
    changeSound(SOUND_NONE);
}

void FungusItem::performAction(Socrates* object) {
    object->sethp(object->gethp() - 20);
}
    
/*
       BACTERIA FUNCTIONS
*/
Bacteria::Bacteria(StudentWorld* world, int imageID, double startX, double startY, int hp, int damage, int steps, int deadsound, int hurtsound) :LivingBeing(world, imageID, startX, startY, hp, 90) {
    m_movementplandistance = 0;
    m_food = 0;
    m_damage = damage;
    m_steps = steps;
    m_deadsound = deadsound;
    m_hurtsound = hurtsound;
}

void Bacteria::findNewDirection() {
    
    if (m_movementplandistance > 0 ) {
        
        m_movementplandistance--;
        if (wp()->checkBlockedMovement(this) || !wp()->insideDish(this, getSteps())) {
            chooseRandomDirection();
            return;
        }
       
        moveAngle(getDirection(), 3);
        /*moveTo(getX() + 3 * cos((getDirection() * PI / 180)), getY() + 3 * sin((getDirection() * PI / 180)));*/
        return;
        
    }
    wp()->turnTowardsFood(this);
}

void Bacteria::chooseRandomDirection() {
    setDirection(randInt(0, 359));
    resetMovementPlan();
}

bool Bacteria::checkFoodSplit(double& newX, double& newY) {
    if (m_food != 3) {
        return false;
    }
    
    newX = getX();
    newY = getY();
    
    if (getX() < VIEW_WIDTH/2) {
        newX += SPRITE_WIDTH/2;
    }
    else if (getX() > VIEW_WIDTH/2) {
        newX -= SPRITE_WIDTH/2;
    }
    
    if (getY() < VIEW_WIDTH/2) {
        newY += SPRITE_WIDTH/2;
    }
    else if (getY() > VIEW_WIDTH/2) {
        newY -= SPRITE_WIDTH/2;
    }
    
    addNewBacteria(newX, newY);
    m_food = 0;
    wp()->bacteriaBorn();
    return true;
}

bool Bacteria::checkFoodOverlap() {
    return wp()->consumeFood(this);
}

bool Bacteria::checkSocratesOverlap() {
    return wp()->checkDamageSocrates(this);
}

void Bacteria::commonBacteriaActions() {
    if (checkSocratesOverlap()) {
        return;
    }
    
    double x;
    double y;
    
    if (checkFoodSplit(x, y)) {
        return;
    }
    
    if (checkFoodOverlap()) {
        return;
    }
}

void Bacteria::doSomething() {
    
    //PRELIM CHECKS
    if (!alive()) {
        return;
    }
    bool shouldChangeDir = shouldChangeDirections();
    
    //SAME FOR EVERYONE
    commonBacteriaActions();
    
    //SAME FOR SALMONELLA, DIFFERENT FOR ECOLI
    if (shouldChangeDir) {
        getNewDirection();
    }
    
}

void Bacteria::hitByProjectile() {
    //damage taken care of by other function

    if (!alive()) {
        return;
    
    }
    
    if (gethp() > 0) {
        wp()->playSound(m_hurtsound);
        return;
    }
    
    setDead();
    wp()->bacteriaDied();
    wp()->playSound(m_deadsound);
    wp()->increaseScore(100);
    
    int foodrand = randInt(0, 1);
    if (foodrand == 0) {
        wp()->addObject(new Food(wp(), getX(), getY()));
    }
}

/*
       SALMONELLA FUNCTIONS
*/

Salmonella::Salmonella(StudentWorld* world, double startX, double startY, int hp, int damage) :Bacteria(world, IID_SALMONELLA, startX, startY, hp, damage, 3, SOUND_SALMONELLA_DIE, SOUND_SALMONELLA_HURT) {
    
}

/*
void Salmonella::moveInSameDirection() {
    if (getMovementPlan() <= 0) {
        return;
    }
    decrementMovementPlan();
}
 */
void Salmonella::getNewDirection() {
    findNewDirection();
    wp()->consumeFood(this);
}


/*
       REGULARSALMONELLA FUNCTIONS
*/
RegularSalmonella::RegularSalmonella(StudentWorld* world, double startX, double startY) :Salmonella(world, startX, startY, 4, 1) {
    
}

void RegularSalmonella::addNewBacteria(double newX, double newY) {
    wp()->addObject(new RegularSalmonella(wp(), newX, newY));
}

/*
       AGGRESSIVE SALMONELLA FUNCTIONS
*/
AggressiveSalmonella::AggressiveSalmonella(StudentWorld* world, double startX, double startY) :Salmonella(world, startX, startY, 10, 2) {
    
}

void AggressiveSalmonella::addNewBacteria(double newX, double newY) {
    wp()->addObject(new AggressiveSalmonella(wp(), newX, newY));
}

bool AggressiveSalmonella::shouldChangeDirections() {
    //if socrates is close enough, then you don't want to change directions again
    return !(wp()->socratesCloseEnough(this, 72));
}

/*
       ECOLI FUNCTIONS
*/
Ecoli::Ecoli(StudentWorld* world, double startX, double startY) :Bacteria(world, IID_ECOLI, startX, startY, 5, 4, 2, SOUND_ECOLI_DIE, SOUND_ECOLI_HURT) {
    
}

void Ecoli::addNewBacteria(double newX, double newY) {
    wp()->addObject(new Ecoli(wp(), newX, newY));
}

void Ecoli::getNewDirection() {
    if (wp()->distanceToSocrates(this) <= 256) {
        setDirection(wp()->directionToSocrates(this));
        for (int i = 0; i < 10; i++) {
            if (wp()->ecoliMove(this)) {
                return;
            }
            setDirection(getDirection() + 10);
        }
    }
}

/*
       PIT FUNCTIONS
*/
Pit::Pit(StudentWorld* world, double startX, double startY) :Actor(world, IID_PIT, startX, startY, 0, 1) {
    m_nregsalmonella = 5;
    m_naggsalmonella = 3;
    m_necoli = 2;
    m_total = 10;
}

void Pit::doSomething() {
    if (!alive()) {
        return;
    }
    
    if (m_total == 0) {
        wp()->pitDied();
        setDead();
    }
    
    int spawnrand = randInt(0, 49);
    
    if (spawnrand == 0) {
        int typerand = randInt(0, 2);
        
        //randomly chooses a bacteria to spawn with equal odds for every bacteria that's still remaining
        for(;;) {
            if (typerand == 0 && m_nregsalmonella != 0) {
                m_nregsalmonella--;
                wp()->addObject(new RegularSalmonella(wp(), getX(), getY()));
                break;
            }
            if (typerand == 1 && m_naggsalmonella != 0) {
                m_naggsalmonella--;
                wp()->addObject(new AggressiveSalmonella(wp(), getX(), getY()));
                break;
            }
            if (typerand == 2 && m_necoli != 0) {
                m_necoli--;
                wp()->addObject(new Ecoli(wp(), getX(), getY()));
                break;
            }
            typerand = randInt(0, 2);
        }
        m_total--;
        wp()->playSound(SOUND_BACTERIUM_BORN);
    }
   
}

/*
       PROJECTILE FUNCTIONS
*/
Projectile::Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int maxdistance, int damage) :Binary(world, imageID, startX, startY, startDirection) {
    m_maxdistance = maxdistance;
    m_damage = damage;
}

//all projectiles have the same format for doing things 
void Projectile::doSomething() {
    if (!alive()) {
        return;
    }
    
    //overlap with damageable object
    if (wp()->damageOverlap(this)) {
        setDead();
        return;
    }
    
    moveAngle(getDirection(), SPRITE_WIDTH);
    m_maxdistance -= SPRITE_WIDTH;
    
    if (m_maxdistance == 0) {
        setDead();
    }
}

/*
       SPRAY FUNCTIONS
*/
Spray::Spray(StudentWorld* world, double startX, double startY, Direction startDirection) :Projectile(world, IID_SPRAY, startX, startY, startDirection, 112, 2) {
    
}

/*
       FLAME FUNCTIONS
*/
Flame::Flame(StudentWorld* world, double startX, double startY, Direction startDirection) :Projectile(world, IID_FLAME, startX, startY, startDirection, 32, 5) {
    
}
