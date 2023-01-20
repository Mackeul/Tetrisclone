#include "Game.h"

Game* Game::gameInstance = 0;

Game* Game::Instance() {
	if (gameInstance == 0) {
		gameInstance = new Game;
	}

	return gameInstance;
}

void Game::Tick() {

	if (m_GameState == GameState::STATE_PLAY) {
		if ((GetTickCount64() - start_time) > (1000 - m_ScoreManager.getScore())) {
			MovePiece(0, 1);
			start_time = GetTickCount64();
		}

		Draw();
	}
}

void Game::Exit() {

	//clean up code goes here
	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);

	m_Map.removeObserver(&m_ScoreManager);

}

bool Game::Init(HWND hWndMain) {

	if (!m_DisplayManager.Init(hWndMain)) {
		return false;
	}

	m_Map.addObserver(&m_ScoreManager);

	// Load the pieces info from file.
	Piece::Load("Pieces.json");

	mciSendString(L"open \"tetris.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);

	m_ScoreManager.loadHighScores();

	NewGame();

	return(true); //return success

}

void Game::NewGame() {

	m_GameState = GameState::STATE_PLAY;

	mciSendString(L"play mp3 repeat", NULL, 0, NULL);

	start_time = GetTickCount64();
	
	m_ScoreManager.resetScore();

	std::srand(GetTickCount());

	m_Map.Setup();

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

void Game::Draw() { //draw the screen

	int xmy = 0, ymx = 0;

	//place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + PREVIEWAREAWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			m_DisplayManager.DrawTile(xmy, ymx, Tile::GREY);
		}
	}

	PrintScore();
	
	if (m_GameState == GameState::STATE_PAUSE) {
		PrintPaused();
	}
	
	//draw the preview piece
	m_sPrePiece.Draw(&m_DisplayManager);

	m_Map.Draw();

	//draw the moving piece 
	m_sPiece.Draw(&m_DisplayManager);

}

void Game::Paint(HWND hwnd) {

	m_DisplayManager.Paint(hwnd);
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
				EndGame();
			}
			else {
				m_Map.IngestPiece(&m_sPiece);
				m_Map.CheckForClearedRow(&m_sPiece);
				SetupNewPiece();
			}
		}
	}

}

void Game::SetupNewPiece() {

	// Current preview piece becomes the piece in play,
	// and we create a new preview piece.

	m_sPiece = m_sPrePiece;
	m_sPiece.setPosition(MAPWIDTH / 2 - 2, -1);  // Replace with m_Map.getCenterX() ?
	m_sPrePiece.Create(m_sPrePiece, std::rand() % 7);
	m_sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);
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
					if (m_Map.tile[x][y] != Tile::BLACK) {
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

void Game::TogglePause() {

	switch(m_GameState) {
	
	case GameState::STATE_PLAY:
		m_GameState = GameState::STATE_PAUSE;
		mciSendString(L"pause mp3", NULL, 0, NULL);
		PrintPaused();
		break;
	
	case GameState::STATE_PAUSE:
		m_GameState = GameState::STATE_PLAY;
		mciSendString(L"resume mp3", NULL, 0, NULL);
		break;

	}
}

void Game::ToggleHighScores() {

	switch (m_GameState) {

	case GameState::STATE_PLAY:
		m_GameState = GameState::STATE_SHOWHIGHSCORE;
		mciSendString(L"pause mp3", NULL, 0, NULL);
		m_ScoreManager.showHighScores(MAPWIDTH, MAPHEIGHT);
		break;

	case GameState::STATE_SHOWHIGHSCORE:
		m_GameState = GameState::STATE_PLAY;
		mciSendString(L"resume mp3", NULL, 0, NULL);
		break;

	}
}

void Game::PrintPaused() {

	//display paused info
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT / 4 + 4, "GAME PAUSED");
	m_DisplayManager.Print(TILESIZE + 4, MAPHEIGHT / 4 + 5, "||");
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT - 2, "PRESS PAUSE");
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT - 1, "TO CONTINUE");
}

void Game::PrintScore() {

	m_DisplayManager.Print(TILESIZE, 0, "SCORE");
	m_DisplayManager.Print(TILESIZE+6, 0, m_ScoreManager.getScore());
}

void Game::handleInput(WPARAM input) {

	switch (m_GameState) {

	case GameState::STATE_PLAY:
		if (input == VK_DOWN) { // check for down arrow key
			MovePiece(0, 1);
		}
		else if (input == VK_UP || input == VK_SPACE) { // check for up arrow key or space bar for block rotation.
			RotatePiece();
		}
		else if (input == VK_LEFT) { // check for left arrow key
			MovePiece(-1, 0);
		}
		else if (input == VK_RIGHT) { // check for right arrow key
			MovePiece(1, 0);
		}
		else if (input == VK_PAUSE) {
			TogglePause();
		}
		else if (input == VK_F1) {
			ToggleHighScores();
		}
		break;

	case GameState::STATE_PAUSE:
		if (input == VK_PAUSE) {
			TogglePause();
		}
		break;

	case GameState::STATE_SHOWHIGHSCORE:
		if (input == VK_F1) {
			ToggleHighScores();
		}
		break;

	case GameState::STATE_END:
		if (input == VK_SPACE || VK_RETURN) {
			NewGame();
		}
		break;

	case GameState::STATE_WRITEHIGHSCORE:
		if (input == VK_RETURN) {
			m_ScoreManager.writeHighScores();
			m_GameState = GameState::STATE_END;
		}
		else {
			m_ScoreManager.handleInput(input);
			m_ScoreManager.getPlayerName(MAPHEIGHT, MAPWIDTH);
		}
		break;

	}
}

void Game::EndGame() {

	int score_position = m_ScoreManager.checkForHighscore();

	if ( score_position < 10) {
		m_GameState = GameState::STATE_WRITEHIGHSCORE;
		mciSendString(L"seek mp3 at 10", NULL, 0, NULL);
		mciSendString(L"play mp3", NULL, 0, NULL);
		m_ScoreManager.getPlayerName(MAPHEIGHT,MAPWIDTH);	
	}
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT - 3, "GAME OVER");
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT - 2, "PRESS SPACE");
	m_DisplayManager.Print(TILESIZE, MAPHEIGHT - 1, "FOR NEW GAME");
	mciSendString(L"seek mp3 to start", NULL, 0, NULL);
}
