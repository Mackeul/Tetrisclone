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

#include "framework.h"
#include "Piece.h"
#include "Subject.h"
#include "ScoreManager.h"
#include "DisplayManager.h"

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

	DisplayManager m_DisplayManager;

	bool GAMEPAUSED = false;

	Piece m_sPrePiece; // preview piece
	Piece m_sPiece; // the 's' prefixes indicate this is a 'structure'
	
	int Map[MAPWIDTH][MAPHEIGHT + 1]; // the game map

	Game() {};

	void DrawMap();
	
	void PrintPaused();

	void PrintScore();

	void SetPieceInMap();
	void CheckForClearedRow();

	void SetupNewPiece();

};