#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//not sure if the following line is allowed either
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int depth);
    virtual ~Actor() {};
    
    //every actor will do something, but not the same thing, so this is pure virtual
    virtual void doSomething() = 0;
    
    //functions that are the same for every actor
    bool alive() {return m_alive;};
    void setDead() {m_alive = false;}
    StudentWorld* wp() {return m_world;}
    
    //classification functions
    virtual bool canConsumeItems() {return false;}
    virtual bool canBeHurtByBacteria() {return false;}
    virtual bool canBlock() {return false;}
    virtual bool isEdible() {return false;}
    virtual bool damageableByProjectiles() {return false;}
    virtual bool hashp() {return false;}
    virtual void takeDamage(int) {};
    virtual void hitByProjectile() {setDead();}
    virtual bool canBeOverlappedCreating() {return true;}

private:
    StudentWorld* m_world;
    bool m_alive;
};

//class hierarchy based on the need for hit points: bacteria and socrates together in one

//pure virtual class with hitpoints
class LivingBeing : public Actor {
public:
    //required functions
    LivingBeing(StudentWorld* world, int startID, double startX, double startY, int hp, Direction startDirection);
    virtual ~LivingBeing() {};
    virtual void doSomething() = 0;
    
    //redefining hashp, since livingbeings have health
    virtual bool hashp() {return true;}

    //functions that pertain to the hp
    void sethp(int newhp);
    int gethp() {return m_hp;}
    void takeDamage(int damage);
    
private:
    int m_hp;
};

class Binary : public Actor {
public:
    //required functions
    Binary(StudentWorld*world, int startID, double startX, double startY, Direction startDirection);
    virtual ~Binary() {};
    virtual void doSomething() = 0;
private:
};

class Socrates : public LivingBeing {
public:
    //required functions
    Socrates(StudentWorld* world);
    virtual ~Socrates() {};
    virtual void doSomething();
    
    //classification redefinition functions
    virtual bool canConsumeItems() {return true;}
    bool canBeHurtByBacteria() {return true;}
    
    //functions pertaining to weapons
    void addFlamethrowerCharges(int newFlamethrowerCharges);
    int getSprays() {return m_sprays;}
    int getFlameCharges() {return m_flamethrowercharges;}
    void checkAlive();
    
private:
    int m_flamethrowercharges;
    int m_sprays;
};

class Dirt : public Binary {
public:
    //required functions
    Dirt(StudentWorld* world, double startX, double startY);
    virtual ~Dirt() {};
    virtual void doSomething() {};
    
    //classification redefinition functions
    virtual bool canBlock() {return true;}
    bool damageableByProjectiles() {return true;}
    
private:
};

class Food : public Binary {
public:
    //required functions
    Food(StudentWorld* world, double startX, double startY);
    virtual ~Food() {};
    void doSomething() {};
    
    //classification redefinition functions
    bool canBeHurtByBacteria() {return true;}
    bool isEdible() {return true;}
    bool canBeOverlappedCreating() {return false;}
    
private:
};

class Projectile : public Binary {
public:
    //required functions
    Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int maxdistance, int damage);
    virtual ~Projectile() {};
    void doSomething();
    
    //functions pertaining to the damage a projectile can deal
    int getDamage() {return m_damage;}
    
private:
    int m_maxdistance;
    int m_damage;
};

class Spray : public Projectile {
public:
    //required functions
    Spray(StudentWorld* world, double startX, double startY, Direction startDirection);
    virtual ~Spray() {};
private:
};

class Flame : public Projectile {
public:
    //required functions
    Flame(StudentWorld* world, double startX, double startY, Direction startDirection);
    virtual ~Flame() {};
private:
};

class Item : public Binary {
public:
    //required functions
    Item(StudentWorld* world, int imageID, double startX, double startY, Direction startDirection, int points);
    virtual ~Item() {};
    virtual void doSomething();
    
    //each item performs a different action on Socrates, so this is pure virtual
    virtual void performAction(Socrates* object) = 0;
    
    //functions pertaining to the unique member variables in item
    void lifetimeExpended();
    int points() {return m_points;}
    int getSound() {return m_sound;}
    void changeSound(int sound) {m_sound = sound;}
    
    //classification redefinition functions
    bool damageableByProjectiles() {return true;}
    
private:
    int m_lifetime;
    int m_points;
    int m_sound;
};

class RestoreHealthItem : public Item {
public:
    //required functions
    RestoreHealthItem(StudentWorld* world, double startX, double startY);
    virtual ~RestoreHealthItem() {};
    
    //performs required action on Socrates
    void performAction(Socrates* object);
    
private:
};

class FlamethrowerItem : public Item {
public:
    //required functions
    FlamethrowerItem(StudentWorld* world, double startX, double startY);
    virtual ~FlamethrowerItem() {};
    
    //performs required action on Socrates
    void performAction(Socrates* object);
    
private:
};

class ExtraLifeItem : public Item {
public:
    //required functions
    ExtraLifeItem(StudentWorld* world, double startX, double startY);
    virtual ~ExtraLifeItem() {};
    
    //perform required action on Socrates
    void performAction(Socrates* object);
private:
};

class FungusItem : public Item {
public:
    //required functions
    FungusItem(StudentWorld* world, double startX, double startY);
    virtual ~FungusItem() {};
    
    //perform required action on Socrates
    void performAction(Socrates* object);
    
private:
};

class Bacteria : public LivingBeing {
public:
    //required functions
    Bacteria(StudentWorld* world, int imageID, double startX, double startY, int hp, int damage, int steps, int deadsound, int hurtsound);
    virtual ~Bacteria() {};
    void doSomething();
    
    //functions for getting a Bacteria to find a new direction to move in
    virtual void findNewDirection();
    void chooseRandomDirection();
    virtual void getNewDirection() = 0;
    
    //adding new Bacteria
    virtual void addNewBacteria(double newX, double newY) = 0;
    bool checkFoodSplit(double& newX, double& newY);
    
    //functions pertaining to unique member variables in Bacteria
    void resetMovementPlan() {m_movementplandistance = 10;}
    void decrementMovementPlan() {m_movementplandistance--;}
    int getMovementPlan() {return m_movementplandistance;}
    void incrementFoodCount() {m_food++;}
    int getDamage() {return m_damage;}
    int getSteps() {return m_steps;}
    
    //check Bacteria overlaps
    bool checkFoodOverlap();
    void hitByProjectile();
    bool checkSocratesOverlap();
    
    //classification redefinition functions
    bool damageableByProjectiles() {return true;}
    
    //classification function to be redefined by child classes
    virtual bool shouldChangeDirections() {return true;}
    
    //all the actions that all bacteria do
    void commonBacteriaActions();

private:
    int m_movementplandistance;
    int m_food;
    int m_damage;
    int m_steps;
    int m_deadsound;
    int m_hurtsound;
};

class Salmonella : public Bacteria {
public:
    //required functions
    Salmonella(StudentWorld* world, double startX, double startY, int hp, int damage);
    virtual ~Salmonella() {};
    
    //functions for salmonella to change directions
    void moveInSameDirection();
    void getNewDirection();
    
private:
};

class RegularSalmonella : public Salmonella {
public:
    //required functions
    RegularSalmonella(StudentWorld* world, double startX, double startY);
    virtual ~RegularSalmonella() {};
    
    //adds a new regular salmonella to the game
    void addNewBacteria(double newX, double newY);

private:
};

class AggressiveSalmonella : public Salmonella {
public:
    
    //required functions
    AggressiveSalmonella(StudentWorld* world, double startX, double startY);
    virtual ~AggressiveSalmonella() {};
    
    //add a new aggressive salmonella to the game
    void addNewBacteria(double newX, double newY);
    
    //redefines the way that this choses whether it should change direction or not
    bool shouldChangeDirections();
    
private:
};

class Ecoli : public Bacteria {
public:
    //required functions
    Ecoli(StudentWorld*, double startX, double startY);
    virtual ~Ecoli() {};
    
    //adds a new ecoli to the game
    void addNewBacteria(double newX, double newY);
    
    //redefines the way this chooses whether it should change direction or not
    void getNewDirection();
    
private:
};

class Pit : public Actor {
public:
    //required functions
    Pit(StudentWorld* world, double startX, double startY);
    virtual ~Pit() {};
    void doSomething();
    
    //classification redefinition function
    bool canBeOverlappedCreating() {return false;}

    
private:
    int m_nregsalmonella;
    int m_naggsalmonella;
    int m_necoli;
    int m_total;
};


#endif // ACTOR_H_
