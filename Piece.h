#pragma once
#include "resource.h"

// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include "bitmapobject.h"
#include "framework.h"

// variables for future bitmap.  We need 9 colors, and 1 for Do Not Draw ... so 10 total.
// note that these values are used for offsets in the blocks.bmp bitmap.
enum Tile : int {
    NODRAW = -1,
    BLACK = 0,
    GREY = 1,
    BLUE = 2,
    RED = 3,
    GREEN = 4,
    YELLOW = 5,
    WHITE = 6,
    STEEL = 7,
    PURPLE = 8
};

struct Piece {
    int tile[4][4];
    int x;
    int y;
	int id;
	std::string name;

    void Create(Piece& aPiece, int blockType);
    void setPosition(int x, int y);
    void Move(int deltaX, int deltaY);
    void Rotate();
    void Delete();

	virtual ~Piece();
	virtual Piece* Make() { return new Piece; }

	static bool Load(const std::string& fileName);

};

