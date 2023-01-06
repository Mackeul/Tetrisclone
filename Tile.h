#pragma once

// variables for future bitmap.  We need 9 colors, and 1 for Do Not Draw ... so 10 total.
// note that these values are used for offsets in the blocks.bmp bitmap.
enum Tile : int {
	NODRAW = -1,
	BLACK = 0,
	GREY = 1,
	BLUE = 2,
	RED = 3,
	GREEN = 4,
	YELLOW = 5,
	WHITE = 6,
	STEEL = 7,
	PURPLE = 8
};

// size of square blocks
const int TILESIZE = 16;
