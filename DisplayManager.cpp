#include <windows.h>

#include "DisplayManager.h"


DisplayManager::DisplayManager() {

}

DisplayManager::~DisplayManager() {

}

bool DisplayManager::Init(HWND hWndMain) {

	//set the client area sizegit l
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

	return true;
}

void DisplayManager::DrawTile(int x, int y, int tile) { // put a tile

	//mask first
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoTiles, tile * TILESIZE, TILESIZE, SRCAND);

	//then image
	BitBlt(bmoMap, x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE, bmoTiles, tile * TILESIZE, 0, SRCPAINT);

}

void DisplayManager::DrawChar(int x, int y, const char letter) { // put a character

	const int letterHeight = 16;
	const int letterWidth = 10;
	int letterTile;
	int letterLine;

	if (isalpha(letter)) {
		letterTile = (int)letter - 65; // Translate the letter character to position in bitmap  ASCII A=65, pos A= 3rd line in bitmap position 0
		letterLine = letterTile / 14;  // We put first 14 letters of alphabet on line 1, the rest on line 2 (after first 2 lines of tiles).
		letterTile = (letterTile % 14);
	}
	else if (isdigit(letter)) {
		letterTile = (int)letter - 48;  // Digits are on line 3.  ASCII 0=48.
		letterLine = 2;
	}
	else if (letter == '|') {  // Special pipe character also on line 3 after digits.   
		letterTile = 11;
		letterLine = 2;
	}
	else {
		letterTile = -1;
		letterLine = -1;
	}

	if (letterTile > -1)
		BitBlt(bmoMap, x * letterWidth, y * letterHeight, letterWidth, letterHeight, bmoTiles, letterTile * letterWidth, letterLine * letterHeight + 2 * TILESIZE, SRCAND);

}

// Print integers up to 6 digits with leading zeroes
void DisplayManager::Print(int x, int y, int number) {
	int i = 6;
	while (i <= 11) {
		int aNum = (int)pow(10, (11 - i));
		DrawChar(x + i, y, (number / aNum) % 10 + '0');
		number = number % aNum;
		i++;
	}
}

//Print strings
void DisplayManager::Print(int x, int y, std::string aString) {
	for (int i = 0; i < aString.length(); i++) {
		DrawChar(x + i, y, aString[i]);
	}
}

void DisplayManager::Paint(HWND hwnd) {

	// painting information
	PAINTSTRUCT ps;

	//start painting
	HDC hdc = BeginPaint(hwnd, &ps);

	//redraw the map
	BitBlt(hdc, 0, 0, TILESIZE * MAPWIDTH + TILESIZE * PREVIEWAREAWIDTH, TILESIZE * MAPHEIGHT, bmoMap, 0, 0, SRCCOPY);

	//end painting
	EndPaint(hwnd, &ps);
}