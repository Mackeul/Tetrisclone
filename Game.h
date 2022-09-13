#pragma once
// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN

#include "resource.h"
#include <windows.h>
#include <iostream>
#include <math.h>
#include "bitmapobject.h"
#include "framework.h"
#include "Piece.h"

// since we're using square blocks, let's onlyuse a single size.
const int TILESIZE = 16;

// now for the map...
const int MAPWIDTH = 10;
const int MAPHEIGHT = 30;
const int PREVIEWAREAWIDTH = 8;

class Game {

public:

	Game(const Game*) = delete;  // Copy constructor... prevents making additional copies of Game.

	static Game* Instance();
	bool Init(HWND);
	void NewGame();
	void Tick();
	void Done();
	void MoveBlock(int, int);
	void RotateBlock();
	void RemoveRow(int);

	void RedrawMap(HWND);
	bool CollisionTest(int nx, int ny,const Piece&);
	bool IsPaused();
	void TogglePause();


protected:


private:

	static Game* gameInstance;

	ULONGLONG start_time;

	int score = 0;

	bool GAMEPAUSED = false;

	Piece sPrePiece; // preview piece
	Piece sPiece; // the 's' prefixes indicate this is a 'structure'

	//map for the program
	BitMapObject bmoMap;
	//block images
	BitMapObject bmoBlocks;
	
	int Map[MAPWIDTH][MAPHEIGHT + 1]; // the game map

	Game() {};

	void DrawMap();

	void DrawTile(int, int, int);
	void DrawChar(int, int, const char);
	void PrintScore();
};