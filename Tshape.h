#pragma once

#include "Piece.h"
class Tshape : public Piece {

public:
	Tshape() {

		//initialize the piece to all blank
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tile[i][j] = Tile::NODRAW;
			}
		}

		tile[1][1] = Tile::STEEL;
		tile[0][2] = Tile::STEEL;
		tile[1][2] = Tile::STEEL;
		tile[2][2] = Tile::STEEL;
	}
};