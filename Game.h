#pragma once
// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>


// To play music
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

#include "resource.h"
#include "framework.h"

#include "Piece.h"
#include "Map.h"
#include "Subject.h"
#include "ScoreManager.h"
#include "DisplayManager.h"
#include "Tile.h"

//const int TILESIZE = 16;

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

	void Paint(HWND);

	bool CollisionTest(const Piece&);

	bool IsPaused();
	void TogglePause();
	void ToggleHighScores();

protected:


private:

	static Game* gameInstance;
	DisplayManager m_DisplayManager;

	ULONGLONG start_time;

	ScoreManager m_ScoreManager = ScoreManager(&m_DisplayManager);

	bool GAMEPAUSED = false;

	Piece m_sPrePiece; // preview piece
	Piece m_sPiece; // the 's' prefixes indicate this is a 'structure'

	Map m_Map = Map(&m_DisplayManager);

	Game() {};

	void Draw();
	
	void PrintPaused();

	void PrintScore();

	void SetupNewPiece();

};