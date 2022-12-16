
#include "Game.h"
#include "string"

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

	if ((GetTickCount64() - start_time) > (1000 - score)) {
		MovePiece(0, 1);
		start_time = GetTickCount64();
	}

	DrawMap();
}

void Game::Done() {
	//clean up code goes here
	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);
}

bool Game::Init(HWND hWndMain) {
		
	//set the client area size
	RECT rcTemp;
		
	SetRect(&rcTemp, 0, 0, (MAPWIDTH + PREVIEWAREAWIDTH) * TILESIZE, MAPHEIGHT * TILESIZE); // 160x480 client area
	AdjustWindowRect(&rcTemp, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, FALSE); //adjust the window size based on desired client area
	SetWindowPos(hWndMain, NULL, 0, 0, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, SWP_NOMOVE); //set the window width and height

	//create map image
	HDC hdc = GetDC(hWndMain);
	bmoMap.Create(hdc, (MAPWIDTH + PREVIEWAREAWIDTH) * TILESIZE, MAPHEIGHT * TILESIZE);
	FillRect(bmoMap, &rcTemp, (HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(hWndMain, hdc);

	bmoTiles.Load(NULL, L"blocks.bmp");

	Piece::Load("Pieces.json");

	NewGame();

	mciSendString(L"open \"tetris.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString(L"play mp3 repeat", NULL, 0, NULL);



	return(true); //return success

}

void Game::NewGame() {

	start_time = GetTickCount64();
	score = 0;

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
	m_sPrePiece.Create(m_sPrePiece, std::rand() % 7);
	m_sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);

	// Creating both pieces one right after the other here at the beginning
	// doesn't usually give enough time for the rand function to give
	// us a new number (ie. new piece), so sleeping a random amount of milliseconds,
	// up to one second max, to account for this.I
	Sleep(std::rand() % 1000);

	//Create the start piece
	m_sPiece.Create(m_sPiece, std::rand() % 7);
	m_sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
	
}

void Game::DrawMap() { //draw the screen

	int xmy = 0, ymx = 0;

	//place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + PREVIEWAREAWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			this->DrawTile(xmy, ymx, Tile::GREY);
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
				DrawTile(m_sPrePiece.x + xmy, m_sPrePiece.y + ymx, m_sPrePiece.tile[xmy][ymx]);
			}
		}
	}

	//draw the map
	//loop through the positions
	for (xmy = 0; xmy < MAPWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			DrawTile(xmy, ymx, Map[xmy][ymx]);
		}
	}

	//draw the moving piece 
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (m_sPiece.tile[xmy][ymx] != Tile::NODRAW) {
				DrawTile(m_sPiece.x + xmy, m_sPiece.y + ymx, m_sPiece.tile[xmy][ymx]);
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

void Game::DrawTile(int x, int y, int tile) { // put a tile

	//mask first
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoTiles, tile * TILESIZE, TILESIZE, SRCAND);

	//then image
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoTiles, tile * TILESIZE, 0, SRCPAINT);

}

void Game::DrawChar(int x, int y, const char letter) { // put a character

	const int letterHeight = 16;
	const int letterWidth = 10;
	int letterTile;
	int letterLine;

	if (isalpha(letter)) {
		letterTile = (int)letter - 65; // Translate the letter character to position in bitmap  ASCII A=65, pos A= 3rd line in bitmap position 0
		letterLine = letterTile / 14;  // We put first 14 letters of alphabet on line 1, the rest on line 2 (after first 2 lines of tiles).
		letterTile = (letterTile % 14);
	} else if (isdigit(letter)) {
		letterTile = (int)letter - 48;  // Digits are on line 3.  ASCII 0=48.
		letterLine = 2;
	} else if (letter == '|') {  // Special pipe character also on line 3 after digits.   
		letterTile = 11;
		letterLine = 2;
	}
	else {
		letterTile = -1;
		letterLine = -1;
	}

	if (letterTile > -1)
	  BitBlt(bmoMap, x * letterWidth, y * letterHeight, letterWidth, letterHeight, bmoTiles, letterTile*letterWidth, letterLine * letterHeight +  2 * TILESIZE, SRCAND);
	
}

void Game::PaintMap(HWND hwnd) {

	// painting information
	PAINTSTRUCT ps;

	//start painting
	HDC hdc = BeginPaint(hwnd, &ps);

	//redraw the map
	BitBlt(hdc, 0, 0, TILESIZE * MAPWIDTH + TILESIZE * PREVIEWAREAWIDTH, TILESIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);

	//end painting
	EndPaint(hwnd, &ps);
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
			score += 10;
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
	Print(TILESIZE, MAPHEIGHT / 4 + 4, "GAME PAUSED");
	Print(TILESIZE + 4, MAPHEIGHT / 4 + 5, "||");
}

void Game::PrintScore() {

	Print(TILESIZE, 0, "SCORE");
	Print(TILESIZE, 0, score);
}

// Print integers up to 6 digits
void Game::Print(int x, int y, int number) {
	int i = 6;
	while (i <= 11) {
		int aNum = (int)pow(10, (11 - i));
		DrawChar(x+i,y, (number / aNum) % 10 + '0');
		number = number % aNum;
		i++;
	}
}

//Print strings
void Game::Print(int x, int y, std::string aString) {
	for (int i = 0; i < aString.length(); i++) {
		DrawChar(x+i, y, aString[i]);
	}
}
