#pragma once
// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN

#include "resource.h"
#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>


// To play music
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

#include "bitmapobject.h"
#include "framework.h"
#include "Piece.h"
#include "Subject.h"
#include "ScoreManager.h"

// since we're using square blocks, let's onlyuse a single size.
const int TILESIZE = 16;

// now for the map...
const int MAPWIDTH = 10;
const int MAPHEIGHT = 30;
const int PREVIEWAREAWIDTH = 8;

class Game : public Subject {

public:

	Game(const Game*) = delete;  // Copy constructor... prevents making additional copies of Game.

	static Game* Instance();
	bool Init(HWND);
	void NewGame();
	void Tick();
	void Done();
	void MovePiece(int deltaX, int deltaY);
	void RotatePiece();
	void RemoveRow(int);

	void PaintMap(HWND);

	bool CollisionTest(const Piece&);

	bool IsPaused();
	void TogglePause();

protected:


private:

	static Game* gameInstance;

	ULONGLONG start_time;

	//int score = 0;

	ScoreManager m_ScoreManager;

	bool GAMEPAUSED = false;

	Piece m_sPrePiece; // preview piece
	Piece m_sPiece; // the 's' prefixes indicate this is a 'structure'

	//map for the program
	BitMapObject bmoMap;

	//block images
	BitMapObject bmoTiles;
	
	int Map[MAPWIDTH][MAPHEIGHT + 1]; // the game map

	Game() {};

	void DrawMap();

	void DrawTile(int, int, int);
	void DrawChar(int, int, const char);
	
	void PrintPaused();

	void Print(int, int, int);
	void Print(int, int, std::string);
	void PrintScore();

	void SetPieceInMap();
	void CheckForClearedRow();

	void SetupNewPiece();

};