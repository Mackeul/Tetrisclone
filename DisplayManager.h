#pragma once
#include <string>

#include "bitmapobject.h"
#include "Tile.h"

class DisplayManager
{

public:

	DisplayManager();
	~DisplayManager();

	bool Init(HWND);

	void Print(int x, int y, int number);
	void Print(int x, int y, std::string aString);

	template<typename T>
	void TPrint(int x, int y, T aValue);

	void DrawTile(int, int, int);

	void Paint(HWND hwnd);

private:
	//map for the program
	BitMapObject bmoMap;

	//block images
	BitMapObject bmoTiles;

	// now for the map...
	const int MAPWIDTH = 10;
	const int MAPHEIGHT = 30;
	const int PREVIEWAREAWIDTH = 8;

	void DrawChar(int, int, const char);
};

