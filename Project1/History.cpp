//
//  History.cpp
//  BadBloodCS32
//
//  Created by David Deng on 1/8/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;
#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 1; r <= nRows; r++) {
        for (int c = 1; c <= nCols; c++) {
            m_grid[r-1][c-1] = '.';
        }
    }
}

bool History::record(int r, int c) {
    if (r <= 0 || c <= 0 || r > MAXROWS || c > MAXCOLS) {
        return false;
    }
    if (m_grid[r-1][c-1] == '.') {
        m_grid[r-1][c-1] = 'A';
    }
    else if (m_grid[r-1][c-1] == 'Z') {
        m_grid[r-1][c-1] = 'Z';
    }
    else {
        m_grid[r-1][c-1]++;
    }
    return true;
}

void History::display() const {
    clearScreen();
    for (int r = 1; r <= m_rows; r++) {
        for (int c = 1; c <= m_cols; c++) {
            cout << m_grid[r-1][c-1];
        }
        cout << endl;
    }
    cout << endl;
}
