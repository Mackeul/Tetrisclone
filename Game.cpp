#include "BitmapObject.h"
#include "Game.h"
#include "Piece.h"

Game* Game::gameInstance = 0;

Game* Game::Instance() {
	if (gameInstance == 0) {
		gameInstance = new Game;
	}

	return gameInstance;
}

void Game::Tick() {

	if (GAMEPAUSED)  //do nothing
		return;

	if ((GetTickCount64() - start_time) > 1000) {
		this->MoveBlock(0, 1);
		start_time = GetTickCount64();
	}

}

void Game::Done() {
	//clean up code goes here
}

bool Game::Init(HWND hWndMain) {
		
	//set the client area size
	RECT rcTemp;
		
	SetRect(&rcTemp, 0, 0, MAPWIDTH * TILESIZE + TILESIZE * GREY, MAPHEIGHT * TILESIZE); // 160x480 client area
	AdjustWindowRect(&rcTemp, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, FALSE); //adjust the window size based on desired client area
	SetWindowPos(hWndMain, NULL, 0, 0, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, SWP_NOMOVE); //set the window width and height

	//create map image
	HDC hdc = GetDC(hWndMain);
	bmoMap.Create(hdc, MAPWIDTH * TILESIZE + TILESIZE * GREY, MAPHEIGHT * TILESIZE);
	FillRect(bmoMap, &rcTemp, (HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(hWndMain, hdc);

	bmoBlocks.Load(NULL, L"blocks.bmp");
	this->NewGame();
	this->DrawMap();

	return(true); //return success

}

void Game::NewGame() {   // Game start?

	int i, j;

	start_time = GetTickCount64();
	GAMESTARTED = false;

	//start out the map
	for (int x = 0; x < MAPWIDTH; x++) {
		for (int y = 0; y < MAPHEIGHT; y++) {
			if (y == MAPHEIGHT) { //makes Y-collision easier
				Map[x][y] = TILEGREY;
			}
			else {
				Map[x][y] = TILEBLACK;
			}
		}
	}
	if (GAMESTARTED) {
		//Set new piece to preview piece
		sPiece = sPrePiece;
	}
	else {
		//Create the first piece
		sPiece.Create(sPiece);
		GAMESTARTED = true;
	}
	sPiece.setPosition(MAPWIDTH / 2 - 2, -1);
	
	//Create the preview piece and set position
	sPrePiece.Create(sPrePiece);
	sPrePiece.setPosition(MAPWIDTH + GREY / 4, GREY / 4);

}

void Game::DrawMap() { //draw the screen

	int xmy = 0, ymx = 0;

	//place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + GREY; xmy++) {
		for (ymx = 0; ymx < MAPHEIGHT; ymx++) {
			this->DrawTile(xmy, ymx, TILEGREY);
		}
	}

	this->DrawChar(MAPWIDTH + GREY + 1, GREY / 4, "A");
	

	//draw the preview block
	for (xmy = 0; xmy < 4; xmy++) {
		for (ymx = 0; ymx < 4; ymx++) {
			if (sPrePiece.size[xmy][ymx] != TILENODRAW) {
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
			if (sPiece.size[xmy][ymx] != TILENODRAW) {
				this->DrawTile(sPiece.x + xmy, sPiece.y + ymx, sPiece.size[xmy][ymx]);
			}
		}
	}

}

void Game::RemoveRow(int row) {
	int x, y;
	int counter = 0;

	for (x = 0; x < MAPWIDTH; x++) {
		for (y = row; y > 0; y--) {
			Map[x][y] = Map[x][y - 1];
		}
	}
}

void Game::DrawTile(int x, int y, int tile) { // put a tile

	//mask first
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, TILESIZE, SRCAND);

	//then image
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, 0, SRCPAINT);

}

void Game::DrawChar(int x, int y, const char* letter) { // put a character

	int tile = 1; //letter;   // Translate the letter character to position in bitmap  A=65

	//mask first
	//maybe not BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, tile * TILESIZE, TILESIZE, SRCAND);

	//then image
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoBlocks, 0, tile * TILESIZE * 3, SRCAND);

}

void Game::RedrawMap(HWND hwnd) {

	// a variable needed for painting information
	PAINTSTRUCT ps;

	//start painting
	HDC hdc = BeginPaint(hwnd, &ps);

	//redraw the map
	BitBlt(hdc, 0, 0, TILESIZE * MAPWIDTH + TILESIZE * GREY, TILESIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);

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
						if (sPiece.size[i][j] != TILENODRAW) {
							Map[sPiece.x + i][sPiece.y + j] = sPiece.size[i][j];
						}
					}
				}
				//check for cleared row!
				for (j = 0; j < MAPHEIGHT; j++) {
					bool filled = true;
					for (i = 0; i < MAPWIDTH; i++) {
						if (Map[i][j] == TILEBLACK) {
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
							sPiece.size[i][j] = TILENODRAW;
						}
					}
				}
				sPiece = sPrePiece;
				sPiece.setPosition(MAPWIDTH / 2 - 2, -1);

				sPrePiece.Create(sPrePiece);
				sPrePiece.setPosition(MAPWIDTH + GREY / 4, GREY / 4);

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
			if (aPiece.size[i][j] != TILENODRAW) {
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
					if (Map[x][y] != TILEBLACK) {
						if (aPiece.size[x - newx][y - newy] != TILENODRAW) {
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
		//display paused graphic
	}

}