//
//  mazequeue.cpp
//  hw2
//
//  Created by David Deng on 2/3/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#include <iostream>
#include <queue>
#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
 public:
   Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
   int r() const { return m_r; }
   int c() const { return m_c; }
 private:
   int m_r;
   int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue<Coord> coordStack;
    Coord first(sr, sc);
    
    coordStack.push(first);
    maze[sr][sc] = '!';
    
    while (coordStack.size() != 0) {
        Coord cur = coordStack.front();
        coordStack.pop();
                
        if (cur.r() == er && cur.c() == ec) {
            return true;
        }
        
        int curRow = cur.r();
        int curCol = cur.c();
        
        //checkes east
        if (curCol < nCols && maze[curRow][curCol + 1] == '.') {
            coordStack.push(Coord(curRow, curCol + 1));
            maze[curRow][curCol + 1] = '!';
        }
        
        //checks south
        if (curRow < nRows && maze[curRow + 1][curCol] == '.') {
            coordStack.push(Coord(curRow + 1, curCol));
            maze[curRow + 1][curCol] = '!';
        }
        
        //checks west
        if (curCol > 0 && maze[curRow][curCol - 1] == '.') {
            coordStack.push(Coord(curRow, curCol - 1));
            maze[curRow][curCol - 1] = '!';
        }
        
        //checks north
        if (curRow > 0 && maze[curRow - 1][curCol] == '.') {
            coordStack.push(Coord(curRow - 1, curCol));
            maze[curRow - 1][curCol] = '!';
        }
    }
    return false;
}




