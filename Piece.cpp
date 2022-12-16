#include <iostream>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"
#include "Log.h"

#include "Piece.h"
#include "Ishape.h"
#include "Jshape.h"
#include "Lshape.h"
#include "Oshape.h"
#include "Sshape.h"
#include "Tshape.h"
#include "Zshape.h"

static std::vector<Piece> all_pieces;

void Piece::Create(Piece& aPiece, int blockType) {

  //   0    1   2   3   4    5   6    
  //   X                             These
  //   X   XX   X  XX   XX  XX   XX  are
  //   X   XX  XXX  XX XX    X   X   block
  //   X                     X   X   types
	LOG_INFO("Create Piece...\n");
	LOG_INFO(all_pieces.size());

	for (auto it = all_pieces.begin(); it < all_pieces.end(); it++) {
		LOG_INFO(it->id);
		if (it->id == blockType) {
			std::stringstream logString;
			logString << "Found piece with id: " << it->id << " block name is:" << it->name << std::endl;
			LOG_INFO(logString.str());
			aPiece = *it;
			break;
		}
	}
	/*
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

	*/

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

bool Piece::Load(const std::string& fileName) {

	{   // Parse the JSON and create the pieces.
		using namespace nlohmann;

		ordered_json j;

		try {
			std::ifstream i(fileName);
			j = ordered_json::parse(i);
		}
		catch (ordered_json::parse_error& ex) {
			std::stringstream logString;
			logString << "parse error at byte " << ex.byte << std::endl;
			LOG_ERROR(logString.str());
		}

		auto pieces = j.at("pieces");

		int n = 0;
		for (auto it = pieces.begin(); it < pieces.end(); it++) {
			Piece tmpPiece;

			//initialize the piece to all blank
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					tmpPiece.tile[i][j] = Tile::NODRAW;
				}
			}
			tmpPiece.id = it->at("id");
			tmpPiece.name = it->at("name");
			tmpPiece.tile[it->at("shape")[0][0]][it->at("shape")[0][1]] = it->at("colorIndex");
			tmpPiece.tile[it->at("shape")[1][0]][it->at("shape")[1][1]] = it->at("colorIndex");
			tmpPiece.tile[it->at("shape")[2][0]][it->at("shape")[2][1]] = it->at("colorIndex");
			tmpPiece.tile[it->at("shape")[3][0]][it->at("shape")[3][1]] = it->at("colorIndex");

			all_pieces.push_back(tmpPiece);
		}

	}

	return true;

}
