#include "Piece.h"

void Piece::Create(Piece& aPiece) {

    int i, j;

  //   0    1   2   3   4    5   6    
  //   X                             These
  //   X   XX   X  XX   XX  XX   XX  are
  //   X   XX  XXX  XX XX    X   X   block
  //   X                     X   X   types
    int blockType;

    //initialize the piece to all blank
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            aPiece.tile[i][j] = Tile::NODRAW;
        }
    }

    blockType = std::rand() % 7;

    switch (blockType) {
    case 0: { //Tower
        aPiece.tile[1][0] = Tile::RED;
        aPiece.tile[1][1] = Tile::RED;
        aPiece.tile[1][2] = Tile::RED;
        aPiece.tile[1][3] = Tile::RED;
        aPiece.y = 0;
    }
            break;
    case 1: { //Box
        aPiece.tile[1][1] = Tile::BLUE;
        aPiece.tile[1][2] = Tile::BLUE;
        aPiece.tile[2][1] = Tile::BLUE;
        aPiece.tile[2][2] = Tile::BLUE;
    }
            break;
    case 2: { //Pyramid
        aPiece.tile[1][1] = Tile::STEEL;
        aPiece.tile[0][2] = Tile::STEEL;
        aPiece.tile[1][2] = Tile::STEEL;
        aPiece.tile[2][2] = Tile::STEEL;

    }
            break;
    case 3: { //Left Leaner
        aPiece.tile[0][1] = Tile::YELLOW;
        aPiece.tile[1][1] = Tile::YELLOW;
        aPiece.tile[1][2] = Tile::YELLOW;
        aPiece.tile[2][2] = Tile::YELLOW;
    }
            break;
    case 4: { //Right Leaner
        aPiece.tile[2][1] = Tile::GREEN;
        aPiece.tile[1][1] = Tile::GREEN;
        aPiece.tile[1][2] = Tile::GREEN;
        aPiece.tile[0][2] = Tile::GREEN;
    }
            break;
    case 5: { // Left Knight
        aPiece.tile[1][1] = Tile::WHITE;
        aPiece.tile[2][1] = Tile::WHITE;
        aPiece.tile[2][2] = Tile::WHITE;
        aPiece.tile[2][3] = Tile::WHITE;
    }
            break;
    case 6: { // Right Knight
        aPiece.tile[2][1] = Tile::PURPLE;
        aPiece.tile[1][1] = Tile::PURPLE;
        aPiece.tile[1][2] = Tile::PURPLE;
        aPiece.tile[1][3] = Tile::PURPLE;

    }
            break;
    }

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
