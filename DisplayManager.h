#pragma once
#include <string>

#include "bitmapobject.h"

class DisplayManager
{

public:

	DisplayManager();
	~DisplayManager();

	bool Init(HWND);

	void DrawTile(int, int, int);
	void DrawChar(int, int, const char);

	void Print(int, int, int);
	void Print(int, int, std::string);

	void PaintMap(HWND hwnd);

private:
	//map for the program
	BitMapObject bmoMap;

	//block images
	BitMapObject bmoTiles;

	// size of square blocks
	const int TILESIZE = 16;

	// now for the map...
	const int MAPWIDTH = 10;
	const int MAPHEIGHT = 30;
	const int PREVIEWAREAWIDTH = 8;
};

