#include "Piece.h"

void Piece::Create(Piece& aPiece) {

    int i, j;

  //   0    1   2   3   4    5   6    
  //   X                             These
  //   X   XX   X  XX   XX  XX   XX  are
  //   X   XX  XXX  XX XX    X   X   block
  //   X                     X   X   types
    int blockType;
  
    std::srand(GetTickCount64());

    //initialize the piece to all blank
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            aPiece.size[i][j] = Tile::NODRAW;
        }
    }

    blockType = std::rand() % 7;

    switch (blockType) {
    case 0: { //Tower
        aPiece.size[1][0] = Tile::RED;
        aPiece.size[1][1] = Tile::RED;
        aPiece.size[1][2] = Tile::RED;
        aPiece.size[1][3] = Tile::RED;
        aPiece.y = 0;
    }
            break;
    case 1: { //Box
        aPiece.size[1][1] = Tile::BLUE;
        aPiece.size[1][2] = Tile::BLUE;
        aPiece.size[2][1] = Tile::BLUE;
        aPiece.size[2][2] = Tile::BLUE;
    }
            break;
    case 2: { //Pyramid
        aPiece.size[1][1] = Tile::STEEL;
        aPiece.size[0][2] = Tile::STEEL;
        aPiece.size[1][2] = Tile::STEEL;
        aPiece.size[2][2] = Tile::STEEL;

    }
            break;
    case 3: { //Left Leaner
        aPiece.size[0][1] = Tile::YELLOW;
        aPiece.size[1][1] = Tile::YELLOW;
        aPiece.size[1][2] = Tile::YELLOW;
        aPiece.size[2][2] = Tile::YELLOW;
    }
            break;
    case 4: { //Right Leaner
        aPiece.size[2][1] = Tile::GREEN;
        aPiece.size[1][1] = Tile::GREEN;
        aPiece.size[1][2] = Tile::GREEN;
        aPiece.size[0][2] = Tile::GREEN;
    }
            break;
    case 5: { // Left Knight
        aPiece.size[1][1] = Tile::WHITE;
        aPiece.size[2][1] = Tile::WHITE;
        aPiece.size[2][2] = Tile::WHITE;
        aPiece.size[2][3] = Tile::WHITE;
    }
            break;
    case 6: { // Right Knight
        aPiece.size[2][1] = Tile::PURPLE;
        aPiece.size[1][1] = Tile::PURPLE;
        aPiece.size[1][2] = Tile::PURPLE;
        aPiece.size[1][3] = Tile::PURPLE;

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
            temp[3 - j][i] = this->size[i][j];
        }
    }

    //Copy rotated piece back into original
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            this->size[i][j] = temp[i][j];
        }
    }
}

void Piece::Move(int x, int y) {

    this->x += x;
    this->y += y;

}

void Piece::Delete() {

    for (int x = 0; x < 4; x++) {
        for (int y = 0;  y < 4; y++) {
            this->size[x][y] = 0;
        }
    }
    this->x = 0;
    this->y = 0;

}
