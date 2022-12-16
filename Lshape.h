#pragma once

#include "Piece.h"
class Lshape : public Piece {

public:
	Lshape() {

		//initialize the piece to all blank
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tile[i][j] = Tile::NODRAW;
			}
		}
		tile[1][1] = Tile::WHITE;
		tile[2][1] = Tile::WHITE;
		tile[2][2] = Tile::WHITE;
		tile[2][3] = Tile::WHITE;

	}
};
