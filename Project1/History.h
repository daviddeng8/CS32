//
//  History.hpp
//  BadBloodCS32
//
//  Created by David Deng on 1/8/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"

class Arena;

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    char m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
};

#endif /* History_h */
