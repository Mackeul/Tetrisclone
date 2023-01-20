#pragma once
#include <vector>
#include "resource.h"

// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include "bitmapobject.h"
#include "framework.h"
#include "DisplayManager.h"
#include "Tile.h"

struct Piece {

    int tile[4][4];
    int x;
    int y;
	int id;
	std::string name;

    void Create(Piece& aPiece, int blockType);
	void Create(Piece& aPiece, std::string blockName);

    void setPosition(int x, int y);
    void Move(int deltaX, int deltaY);
    void Rotate();
    void Delete();

	virtual ~Piece();
	virtual Piece* Make() { return new Piece; }

	static bool Load(const std::string& fileName);
	static int NumPieces();

	void Draw(DisplayManager* dm);

};

