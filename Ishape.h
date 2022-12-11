#pragma once
#include "Piece.h"
class Ishape : public Piece {

public:
	Ishape() {

		//initialize the piece to all blank
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tile[i][j] = Tile::NODRAW;
			}
		}

		tile[1][0] = Tile::RED;
		tile[1][1] = Tile::RED;
		tile[1][2] = Tile::RED;
		tile[1][3] = Tile::RED;
	}
};