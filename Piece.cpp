#include "Piece.h"
#include "Ishape.h"
#include "Jshape.h"
#include "Lshape.h"
#include "Oshape.h"
#include "Sshape.h"
#include "Tshape.h"
#include "Zshape.h"

void Piece::Create(Piece& aPiece, int blockType) {

    int i, j;

  //   0    1   2   3   4    5   6    
  //   X                             These
  //   X   XX   X  XX   XX  XX   XX  are
  //   X   XX  XXX  XX XX    X   X   block
  //   X                     X   X   types

	Piece* tmpPiece;

    switch (blockType) {
    case 0: {
		tmpPiece = new Ishape;
		aPiece = *tmpPiece;
    }
            break;
    case 1: { //O
		tmpPiece = new Oshape;
		aPiece = *tmpPiece;
    }
            break;
    case 2: { //T
		tmpPiece = new Tshape;
		aPiece = *tmpPiece;

    }
            break;
    case 3: { //L or 7
		tmpPiece = new Lshape;
		aPiece = *tmpPiece;
    }
            break;
    case 4: { //J or r
		tmpPiece = new Jshape;
		aPiece = *tmpPiece;
    }
            break;
    case 5: { // Z
		tmpPiece = new Zshape;
		aPiece = *tmpPiece;
    }
            break;
    case 6: { // S
		tmpPiece = new Sshape;
		aPiece = *tmpPiece;
    }
            break;
    }

	tmpPiece = nullptr;
	delete tmpPiece;

}

void Piece::setPosition(int x, int y) {
    
    this->x = x;
    this->y = y;

}

void Piece::Rotate() {

    int i, j, temp[4][4];

    //copy and rotate the piece to the temporary array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[3 - j][i] = tile[i][j];
        }
    }

    //Copy rotated piece back into original
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tile[i][j] = temp[i][j];
        }
    }
}

void Piece::Move(int deltaX, int deltaY) {

    x += deltaX;
    y += deltaY;

}

void Piece::Delete() {

    for (int x = 0; x < 4; x++) {
        for (int y = 0;  y < 4; y++) {
            tile[x][y] = 0;
        }
    }
    x = 0;
    y = 0;

}

Piece::~Piece() {

}
