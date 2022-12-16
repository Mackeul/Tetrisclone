#pragma once

#include "Piece.h"
class Sshape : public Piece {

public:
	Sshape() {

		//initialize the piece to all blank
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tile[i][j] = Tile::NODRAW;
			}
		}

		tile[2][1] = Tile::GREEN;
		tile[1][1] = Tile::GREEN;
		tile[1][2] = Tile::GREEN;
		tile[0][2] = Tile::GREEN;
	}
};