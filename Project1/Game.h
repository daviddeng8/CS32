//
//  Game.h
//  BadBloodCS32
//
//  Created by David Deng on 1/8/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <string>

class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};


#endif /* Game_h */
