//
//  Player.hpp
//  BadBloodCS32
//
//  Created by David Deng on 1/8/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <string>
class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonVial();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif /* Player_h */
