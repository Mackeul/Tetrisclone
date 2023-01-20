#pragma once

#include "DisplayManager.h"
#include "Tile.h"
#include "Piece.h"
#include "Subject.h"

class Map : public Subject
{
private:

	static const int MAPWIDTH = 10;
	static const int MAPHEIGHT = 30;

	DisplayManager* m_dm;

public:

	Map(DisplayManager* dm);
	~Map();

	void Setup();
	void Draw();
	void IngestPiece(Piece* piece);
	void RemoveRow(int row);
	void CheckForClearedRow(Piece* piece);

	int GetWidth() { return MAPWIDTH; }
	int GetHeight() { return MAPHEIGHT; }

	int tile[MAPWIDTH][MAPHEIGHT];



};

