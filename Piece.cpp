#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "nlohmann/json.hpp"
#include "Log.h"

#include "Piece.h"

static std::vector<Piece> all_pieces;

void Piece::Create(Piece& aPiece, int blockType) {

	for (auto it = all_pieces.begin(); it < all_pieces.end(); it++) {
		if (it->id == blockType) {
			aPiece = *it;
			break;
		}
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

Piece::~Piece() {

}

bool Piece::Load(const std::string& fileName) {  //Load pieces information from JSON file.

	std::stringstream logString;

	{   // Parse the JSON and create the pieces.
		using namespace nlohmann;

		ordered_json j;

		try {
			std::ifstream i(fileName);
			j = ordered_json::parse(i);
		}
		catch (ordered_json::parse_error& ex) {
			logString.str("");
			logString << "parse error at byte " << ex.byte << std::endl;
			LOG_ERROR(logString.str());
		}

		auto pieces = j.at("pieces");

		int n = 0;
		for (auto pieceIterator = pieces.begin(); pieceIterator < pieces.end(); pieceIterator++) {
			Piece tmpPiece;

			tmpPiece.id = pieceIterator->at("id");
			tmpPiece.name = pieceIterator->at("name");
			
			//initialize the piece to all blank
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					tmpPiece.tile[x][y] = Tile::NODRAW;
				}
			}

			auto shape = pieceIterator->at("shape");

			for (auto shapeIterator = shape.begin(); shapeIterator < shape.end(); shapeIterator++) {
				tmpPiece.tile[shapeIterator->at(0)][shapeIterator->at(1)] = pieceIterator->at("colorIndex");
			}

			all_pieces.push_back(tmpPiece);
		}
	}

	return true;

}

int Piece::NumPieces() {

	return all_pieces.size();
}
