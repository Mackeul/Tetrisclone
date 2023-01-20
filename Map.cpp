#include "Map.h"


Map::Map(DisplayManager* dm)
	: m_dm(dm) {

}

Map::~Map() {

}

void Map::Setup() {

	for (int x = 0; x < MAPWIDTH; x++) {
		for (int y = 0; y <= MAPHEIGHT; y++) {
			if (y == MAPHEIGHT) { //makes Y-collision easier
				tile[x][y] = Tile::GREY;
			}
			else {
				tile[x][y] = Tile::BLACK;
			}
		}
	}
}

void Map::Draw() {
	
	for (int xmy = 0; xmy < MAPWIDTH; xmy++) {
		for (int ymx = 0; ymx < MAPHEIGHT; ymx++) {
			m_dm->DrawTile(xmy, ymx, tile[xmy][ymx]);
		}
	}
}

void Map::IngestPiece(Piece* aPiece) {

	// Once a piece cannot move anymore, 
	// it becomes part of the map.  This is where we set it in.

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (aPiece->tile[i][j] != Tile::NODRAW) {
				tile[aPiece->x + i][aPiece->y + j] = aPiece->tile[i][j];
			}
		}
	}
}

void Map::RemoveRow(int row) {

	int x, y;

	for (x = 0; x < MAPWIDTH; x++) {
		for (y = row; y > 0; y--) {
			tile[x][y] = tile[x][y - 1];
		}
	}

}

void Map::CheckForClearedRow(Piece* piece) {

	bool killblock = false;
	int i, j;

	for (j = 0; j < MAPHEIGHT; j++) {
		bool filled = true;
		for (i = 0; i < MAPWIDTH; i++) {
			if (tile[i][j] == Tile::BLACK) {
				filled = false;
			}
		}
		if (filled) {
			RemoveRow(j);
			notify(Event::ROW_CLEARED);  // add num rows cleared to this
			killblock = true;
		}
	}
	if (killblock) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				piece->tile[i][j] = Tile::NODRAW;
			}
		}
	}
}