#include <string>

#include "Game.h"

Game* Game::gameInstance = 0;

Game* Game::Instance() {
	if (gameInstance == 0) {
		gameInstance = new Game;
	}

	return gameInstance;
}

void Game::Tick() {

	if (IsPaused())  //do nothing
		return;

	if ((GetTickCount64() - start_time) > (1000 - m_ScoreManager.getScore())) {
		MovePiece(0, 1);
		start_time = GetTickCount64();
	}

	DrawMap();
}

void Game::Done() {

	//clean up code goes here
	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);

	removeObserver(&m_ScoreManager);

}

bool Game::Init(HWND hWndMain) {

	if (!m_DisplayManager.Init(hWndMain)) {
		return false;
	}

	addObserver(&m_ScoreManager);

	// Load the pieces info from file.
	Piece::Load("Pieces.json");

	//m_ScoreManager.loadHighScores();

	NewGame();

	mciSendString(L"open \"tetris.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString(L"play mp3 repeat", NULL, 0, NULL);

	return(true); //return success

}

void Game::NewGame() {

	start_time = GetTickCount64();
	
	m_ScoreManager.resetScore();

	std::srand(GetTickCount64());
	
	//start out the map
	for (int x = 0; x < MAPWIDTH; x++) {
		for (int y = 0; y <= MAPHEIGHT; y++) {
			if (y == MAPHEIGHT) { //makes Y-collision easier
				Map[x][y] = Tile::GREY;
			}
			else {
				Map[x][y] = Tile::BLACK;
			}
		}
	}

	//Create the preview piece and set position
	int randomPiece = std::rand() % Piece::NumPieces();
	m_sPrePiece.Create(m_sPrePiece, randomPiece);
	m_sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);

	// Creating both pieces one right after the other here at the beginning doesn't usually give enough time for the rand function to give
	// us a new number (ie. new piece), so sleeping a random amount of milliseconds, up to one second max, to account for this.
	Sleep(std::rand() % 1000);

	//Create the start piece
	randomPiece = std::rand() % Piece::NumPieces();
	m_sPiece.Create(m_sPiece, randomPiece);
	m_sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
	
}

void Game::DrawMap() { //draw the screen

	int xmy = 0, ymx = 0;

	//place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + PREVIEWAREAWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			m_DisplayManager.DrawTile(xmy, ymx, Tile::GREY);
		}
	}

	PrintScore();
	
	if (IsPaused()) {
		PrintPaused();
	}
	
	//draw the preview piece
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (m_sPrePiece.tile[xmy][ymx] != Tile::NODRAW) {
				m_DisplayManager.DrawTile(m_sPrePiece.x + xmy, m_sPrePiece.y + ymx, m_sPrePiece.tile[xmy][ymx]);
			}
		}
	}

	//draw the map
	//loop through the positions
	for (xmy = 0; xmy < MAPWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			m_DisplayManager.DrawTile(xmy, ymx, Map[xmy][ymx]);
		}
	}

	//draw the moving piece 
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (m_sPiece.tile[xmy][ymx] != Tile::NODRAW) {
				m_DisplayManager.DrawTile(m_sPiece.x + xmy, m_sPiece.y + ymx, m_sPiece.tile[xmy][ymx]);
			}
		}
	}

}

void Game::RemoveRow(int row) {

	int x, y;

	for (x = 0; x < MAPWIDTH; x++) {
		for (y = row; y > 0; y--) {
			Map[x][y] = Map[x][y - 1];
		}
	}

}

void Game::PaintMap(HWND hwnd) {

	m_DisplayManager.PaintMap(hwnd);
}

void Game::MovePiece(int deltaX, int deltaY) {

	Piece tempPiece = m_sPiece;

	tempPiece.Move(deltaX, deltaY);

	if (!CollisionTest(tempPiece)) {
		m_sPiece = tempPiece;
	}
	else {
		if (deltaY == 1) {
			// Collision detected while moving piece down, piece can no longer move.
			if (tempPiece.y < 1) {
				//Game Over. Start new game.
				NewGame();
			}
			else {
				SetPieceInMap();
				CheckForClearedRow();
				SetupNewPiece();
			}
		}
	}

}

void Game::SetupNewPiece() {

	// Current preview piece becomes the piece in play,
	// and we create a new preview piece.

	m_sPiece = m_sPrePiece;
	m_sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
	m_sPrePiece.Create(m_sPrePiece, std::rand() % 7);
	m_sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);
}

void Game::CheckForClearedRow() {
	
	bool killblock = false;
	int i, j;

	for (j = 0; j < MAPHEIGHT; j++) {
		bool filled = true;
		for (i = 0; i < MAPWIDTH; i++) {
			if (Map[i][j] == Tile::BLACK) {
				filled = false;
			}
		}
		if (filled) {
			RemoveRow(j);
			notify(Event::ROW_CLEARED);
			killblock = true;
		}
	}
	if (killblock) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				m_sPiece.tile[i][j] = Tile::NODRAW;
			}
		}
	}
}

void Game::SetPieceInMap() {

	// Once a piece cannot move anymore, 
	// it becomes part of the map.  This is where we set it in.

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (m_sPiece.tile[i][j] != Tile::NODRAW) {
				Map[m_sPiece.x + i][m_sPiece.y + j] = m_sPiece.tile[i][j];
			}
		}
	}
}

void Game::RotatePiece() {

	Piece tempPiece = m_sPiece;

	//Rotate temp piece
	tempPiece.Rotate();
	
	//Check temp piece for collision
	if (!CollisionTest(tempPiece)) {
		//If no collision assign tempPiece to m_sPiece to complete the rotation
		m_sPiece = tempPiece;
	}

}

bool Game::CollisionTest(const Piece& aPiece) {

	//Boundary collision
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (aPiece.tile[i][j] != Tile::NODRAW) {
				if (aPiece.x + i < 0 || aPiece.x + i > MAPWIDTH - 1
					|| aPiece.y + j < 0 || aPiece.y + j > MAPHEIGHT - 1) {
					return true;
				}
			}
		}
	}

	//Block collision
	for (int x = 0; x < MAPWIDTH; x++) {
		for (int y = 0; y < MAPHEIGHT; y++) {
			if (x >= aPiece.x && x < aPiece.x + 4) {
				if (y >= aPiece.y && y < aPiece.y + 4) {
					if (Map[x][y] != Tile::BLACK) {
						if (aPiece.tile[x - aPiece.x][y - aPiece.y] != Tile::NODRAW) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Game::IsPaused() {

	return GAMEPAUSED;
}

void Game::TogglePause() {

	GAMEPAUSED = !GAMEPAUSED;
	if (GAMEPAUSED) {
		mciSendString(L"pause mp3", NULL, 0, NULL);
		PrintPaused();
	}
	else {
		mciSendString(L"resume mp3", NULL, 0, NULL);
	}
}

void Game::PrintPaused() {

	//display paused info
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT / 4 + 4, "GAME PAUSED");
	m_DisplayManager.Print(TILESIZE + 4, MAPHEIGHT / 4 + 5, "||");
}

void Game::PrintScore() {

	m_DisplayManager.Print(TILESIZE, 0, "SCORE");
	m_DisplayManager.Print(TILESIZE, 0, m_ScoreManager.getScore());
}