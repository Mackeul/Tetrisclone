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
            aPiece.size[i][j] = TILENODRAW;
        }
    }

    blockType = std::rand() % 7;

    switch (blockType) {
    case 0: { //Tower
        aPiece.size[1][0] = TILERED;
        aPiece.size[1][1] = TILERED;
        aPiece.size[1][2] = TILERED;
        aPiece.size[1][3] = TILERED;
        aPiece.y = 0;
    }
            break;
    case 1: { //Box
        aPiece.size[1][1] = TILEBLUE;
        aPiece.size[1][2] = TILEBLUE;
        aPiece.size[2][1] = TILEBLUE;
        aPiece.size[2][2] = TILEBLUE;
    }
            break;
    case 2: { //Pyramid
        aPiece.size[1][1] = TILESTEEL;
        aPiece.size[0][2] = TILESTEEL;
        aPiece.size[1][2] = TILESTEEL;
        aPiece.size[2][2] = TILESTEEL;

    }
            break;
    case 3: { //Left Leaner
        aPiece.size[0][1] = TILEYELLOW;
        aPiece.size[1][1] = TILEYELLOW;
        aPiece.size[1][2] = TILEYELLOW;
        aPiece.size[2][2] = TILEYELLOW;
    }
            break;
    case 4: { //Right Leaner
        aPiece.size[2][1] = TILEGREEN;
        aPiece.size[1][1] = TILEGREEN;
        aPiece.size[1][2] = TILEGREEN;
        aPiece.size[0][2] = TILEGREEN;
    }
            break;
    case 5: { // Left Knight
        aPiece.size[1][1] = TILEWHITE;
        aPiece.size[2][1] = TILEWHITE;
        aPiece.size[2][2] = TILEWHITE;
        aPiece.size[2][3] = TILEWHITE;
    }
            break;
    case 6: { // Right Knight
        aPiece.size[2][1] = TILEPURPLE;
        aPiece.size[1][1] = TILEPURPLE;
        aPiece.size[1][2] = TILEPURPLE;
        aPiece.size[1][3] = TILEPURPLE;

    }
            break;
    }


}

void Piece::setPosition(int x, int y) {
    
    this->x = x;
    this->y = y;

}
/*
void Piece::Create_orig() {
        
    int blockType;
    int i, j;

    //   0   1   2   3   4    5   6    
    //   X                             These
    //   X   XX   X  XX   XX  XX   XX  are
    //   X   XX  XXX  XX XX    X   X   block
    //   X                     X   X   types


    // begin game!  make generate a block and then one in preview

    srand(GetTickCount64());

    //initialize the piece to all blank
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            sPiece.size[i][j] = TILENODRAW;
        }
    }

    sPiece.x = MAPWIDTH / 2 - 2;
    sPiece.y = -1;

    //let's see if the game's started yet
    if (GAMESTARTED == false) {

        //guess not...
        //Generate a piece right off
        //From now on, use previous preview block.

        GAMESTARTED = TRUE;

        blockType = rand() % 7;

        switch (blockType) {
        case 0: { //Tower
            sPiece.size[1][0] = TILERED;
            sPiece.size[1][1] = TILERED;
            sPiece.size[1][2] = TILERED;
            sPiece.size[1][3] = TILERED;
            sPiece.y = 0;
        }
                break;
        case 1: { //Box
            sPiece.size[1][1] = TILEBLUE;
            sPiece.size[1][2] = TILEBLUE;
            sPiece.size[2][1] = TILEBLUE;
            sPiece.size[2][2] = TILEBLUE;
        }
                break;
        case 2: { //Pyramid
            sPiece.size[1][1] = TILESTEEL;
            sPiece.size[0][2] = TILESTEEL;
            sPiece.size[1][2] = TILESTEEL;
            sPiece.size[2][2] = TILESTEEL;

        }
                break;
        case 3: { //Left Leaner
            sPiece.size[0][1] = TILEYELLOW;
            sPiece.size[1][1] = TILEYELLOW;
            sPiece.size[1][2] = TILEYELLOW;
            sPiece.size[2][2] = TILEYELLOW;
        }
                break;
        case 4: { //Right Leaner
            sPiece.size[2][1] = TILEGREEN;
            sPiece.size[1][1] = TILEGREEN;
            sPiece.size[1][2] = TILEGREEN;
            sPiece.size[0][2] = TILEGREEN;
        }
                break;
        case 5: { // Left Knight
            sPiece.size[1][1] = TILEWHITE;
            sPiece.size[2][1] = TILEWHITE;
            sPiece.size[2][2] = TILEWHITE;
            sPiece.size[2][3] = TILEWHITE;
        }
                break;
        case 6: { // Right Knight
            sPiece.size[2][1] = TILEPURPLE;
            sPiece.size[1][1] = TILEPURPLE;
            sPiece.size[1][2] = TILEPURPLE;
            sPiece.size[1][3] = TILEPURPLE;

        }
                break;
        }

    }
    else {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                sPiece.size[i][j] = sPrePiece.size[i][j];
            }
        }
    }

    blockType = rand() % 7;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            sPrePiece.size[i][j] = TILENODRAW;
        }
    }

    sPrePiece.x = MAPWIDTH + GREY / 4;
    sPrePiece.y = GREY / 4;

    switch (blockType) {
    case 0: { //Tower
        sPrePiece.size[1][0] = TILERED;
        sPrePiece.size[1][1] = TILERED;
        sPrePiece.size[1][2] = TILERED;
        sPrePiece.size[1][3] = TILERED;
    }
            break;
    case 1: { //Box
        sPrePiece.size[1][1] = TILEBLUE;
        sPrePiece.size[1][2] = TILEBLUE;
        sPrePiece.size[2][1] = TILEBLUE;
        sPrePiece.size[2][2] = TILEBLUE;
    }
            break;
    case 2: { //Pyramid
        sPrePiece.size[1][1] = TILESTEEL;
        sPrePiece.size[0][2] = TILESTEEL;
        sPrePiece.size[1][2] = TILESTEEL;
        sPrePiece.size[2][2] = TILESTEEL;

    }
            break;
    case 3: { //Left Leaner
        sPrePiece.size[0][1] = TILEYELLOW;
        sPrePiece.size[1][1] = TILEYELLOW;
        sPrePiece.size[1][2] = TILEYELLOW;
        sPrePiece.size[2][2] = TILEYELLOW;
    }
            break;
    case 4: { //Right Leaner
        sPrePiece.size[2][1] = TILEGREEN;
        sPrePiece.size[1][1] = TILEGREEN;
        sPrePiece.size[1][2] = TILEGREEN;
        sPrePiece.size[0][2] = TILEGREEN;
    }
            break;
    case 5: { // Left Knight
        sPrePiece.size[1][1] = TILEWHITE;
        sPrePiece.size[2][1] = TILEWHITE;
        sPrePiece.size[2][2] = TILEWHITE;
        sPrePiece.size[2][3] = TILEWHITE;
    }
            break;
    case 6: { // Right Knight
        sPrePiece.size[2][1] = TILEPURPLE;
        sPrePiece.size[1][1] = TILEPURPLE;
        sPrePiece.size[1][2] = TILEPURPLE;
        sPrePiece.size[1][3] = TILEPURPLE;

    }
            break;
    }

    DrawMap();
}

*/

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
