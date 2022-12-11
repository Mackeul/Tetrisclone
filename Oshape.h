#pragma once

#include "Piece.h"
class Oshape : public Piece {

public:
	Oshape() {

		//initialize the piece to all blank
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tile[i][j] = Tile::NODRAW;
			}
		}

		tile[1][1] = Tile::BLUE;
		tile[1][2] = Tile::BLUE;
		tile[2][1] = Tile::BLUE;
		tile[2][2] = Tile::BLUE;
	}
};