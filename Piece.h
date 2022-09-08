#pragma once
#include "resource.h"

// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include "bitmapobject.h"
#include "framework.h"

// variables for future bitmap.  We need 9 colors, and 1 for Do Not Draw ... so 10 total.
const int TILENODRAW = -1;
const int TILEBLACK = 0;
const int TILEGREY = 1;
const int TILEBLUE = 2;
const int TILERED = 3;
const int TILEGREEN = 4;
const int TILEYELLOW = 5;
const int TILEWHITE = 6;
const int TILESTEEL = 7;
const int TILEPURPLE = 8;

struct Piece {
    int size[4][4];
    int x;
    int y;

    void Create(Piece&);
    void setPosition(int, int);
    void Move(int , int );
    void Rotate();

};

