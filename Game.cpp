
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

	if (this->IsPaused())  //do nothing
		return;

	if ((GetTickCount64() - start_time) > (1000 - this->score)) {
		this->MoveBlock(0, 1);
		start_time = GetTickCount64();
	}

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

	bmoBlocks.Load(NULL, L"blocks.bmp");
	this->NewGame();

	mciSendString(L"open \"tetris.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString(L"play mp3 repeat", NULL, 0, NULL);

	return(true); //return success

}

void Game::NewGame() {   // Game start?

	this->start_time = GetTickCount64();
	this->score = 0;
	
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
	sPrePiece.Create(sPrePiece);
	sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);
	this->DrawMap();

	sPiece.Create(sPiece);
	sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
	this->DrawMap();
	 
}

void Game::DrawMap() { //draw the screen

	int xmy = 0, ymx = 0;

	//place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + PREVIEWAREAWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			this->DrawTile(xmy, ymx, Tile::GREY);
		}
	}

	this->PrintScore();
	if (this->IsPaused()) {
		this->PrintPaused();
	}
	
	//draw the preview block
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (sPrePiece.size[xmy][ymx] != Tile::NODRAW) {
				this->DrawTile(sPrePiece.x + xmy, sPrePiece.y + ymx, sPrePiece.size[xmy][ymx]);
			}
		}
	}

	//draw the map
	//loop through the positions
	for (xmy = 0; xmy < MAPWIDTH; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			this->DrawTile(xmy, ymx, Map[xmy][ymx]);
		}
	}

	//draw the moving block 
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (sPiece.size[xmy][ymx] != Tile::NODRAW) {
				this->DrawTile(sPiece.x + xmy, sPiece.y + ymx, sPiece.size[xmy][ymx]);
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

	this->score += 10;
}

void Game::DrawTile(int x, int y, int tile) { // put a tile

	//mask first
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, TILESIZE, SRCAND);

	//then image
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, 0, SRCPAINT);

}

void Game::DrawChar(int x, int y, const char letter) { // put a character

	const int letterHeight = 16;
	const int letterWidth = 10;
	int letterTile;
	int letterLine;

	if (isalpha(letter)) {
		letterTile = (int)letter - 65; // Translate the letter character to position in bitmap  ASCII A=65, pos A= 3rd line in bitmap position 0
		letterLine = letterTile / 14;  // We put 14 first letters on line 1, the rest on line 2 (after first 2 lines of tiles).
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
	  BitBlt(bmoMap, x * letterWidth, y * letterHeight, letterWidth, letterHeight, bmoBlocks, letterTile*letterWidth, letterLine * letterHeight +  2 * TILESIZE, SRCAND);
	
}

void Game::RedrawMap(HWND hwnd) {

	// a variable needed for painting information
	PAINTSTRUCT ps;

	//start painting
	HDC hdc = BeginPaint(hwnd, &ps);

	//redraw the map
	BitBlt(hdc, 0, 0, TILESIZE * MAPWIDTH + TILESIZE * PREVIEWAREAWIDTH, TILESIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);

	//end painting
	EndPaint(hwnd, &ps);
}

void Game::MoveBlock(int x, int y) {

	if (this->CollisionTest(x, y, sPiece)) {
		if (y == 1) {
			if (sPiece.y < 1) {
				//you lose!  new game.
				this->NewGame();
			}
			else {
				bool killblock = false;
				int i, j;

				//new block time!  add this one to the list!
				for (i = 0; i < 4; i++) {
					for (j = 0; j < 4; j++) {
						if (sPiece.size[i][j] != Tile::NODRAW) {
							Map[sPiece.x + i][sPiece.y + j] = sPiece.size[i][j];
						}
					}
				}
				//check for cleared row!
				for (j = 0; j < MAPHEIGHT; j++) {
					bool filled = true;
					for (i = 0; i < MAPWIDTH; i++) {
						if (Map[i][j] == Tile::BLACK) {
							filled = false;
						}
					}
					if (filled) {
						this->RemoveRow(j);
						killblock = true;
					}
				}
				if (killblock) {
					for (i = 0; i < 4; i++) {
						for (j = 0; j < 4; j++) {
							sPiece.size[i][j] = Tile::NODRAW;
						}
					}
				}
				sPiece = sPrePiece;
				sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
				sPrePiece.Create(sPrePiece);
				sPrePiece.setPosition(MAPWIDTH + PREVIEWAREAWIDTH / 4, PREVIEWAREAWIDTH / 4);

			}
		}
	}
	else {
		sPiece.Move(x, y);
	}
	this->DrawMap();
}

void Game::RotateBlock() {

	Piece tempPiece = sPiece;

	//Rotate sPiece into temp piece
	tempPiece.Rotate();
	
	//Check temp piece for collision
	if (!this->CollisionTest(0, 0, tempPiece)) {
		//If no collision assign tempPiece to sPiece to complete the rotation
		sPiece = tempPiece;
	}
	this->DrawMap();
}

bool Game::CollisionTest(int nx, int ny,const Piece& aPiece) {

	int newx = aPiece.x + nx;
	int newy = aPiece.y + ny;

	int i, j, x, y;


	//Boundary collision
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (aPiece.size[i][j] != Tile::NODRAW) {
				if (newx + i < 0 || newx + i > MAPWIDTH - 1
					|| newy + j < 0 || newy + j > MAPHEIGHT - 1) {
					return true;
				}
			}
		}
	}

	//Block collision
	for (x = 0; x < MAPWIDTH; x++) {
		for (y = 0; y < MAPHEIGHT; y++) {
			if (x >= newx && x < newx + 4) {
				if (y >= newy && y < newy + 4) {
					if (Map[x][y] != Tile::BLACK) {
						if (aPiece.size[x - newx][y - newy] != Tile::NODRAW) {
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
		this->PrintPaused();
	}
	else {
		mciSendString(L"resume mp3", NULL, 0, NULL);
	}
}

void Game::PrintPaused() {

	//display paused info
	this->Print(TILESIZE, MAPHEIGHT / 4 + 4, "GAME PAUSED");
	this->Print(TILESIZE + 4, MAPHEIGHT / 4 + 5, "||");
}

void Game::PrintScore() {

	this->Print(TILESIZE, 0, "SCORE");
	this->Print(TILESIZE, 0, score);
}

//Could I use a template for the Print function instead?  
//Would it be better than these simple overloaded functions?

// Print integers up to 6 digits
void Game::Print(int x, int y, int number) {
	int i = 6;
	while (i <= 11) {
		int aNum = (int)pow(10, (11 - i));
		this->DrawChar(x+i,y, (number / aNum) % 10 + '0');
		number = number % aNum;
		i++;
	}
}

//Print strings
void Game::Print(int x, int y, std::string aString) {
	for (int i = 0; i < aString.length(); i++) {
		this->DrawChar(x+i, y, aString[i]);
	}
}
