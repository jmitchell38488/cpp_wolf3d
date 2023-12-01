#ifndef GAME_MAP
#define GAME_MAP
#pragma once

#include <vector>
#include <olcPixelGameEngine.h>
#include "Definitions.h"

struct GameMap {
	std::vector<uint8_t> tiles;
	std::vector<olc::vf2d> coords;

	bool checkIntercept(olc::vi2d vec) {
		for (auto coord : coords) {
			if (vec.x == coord.x && vec.y == coord.y)
				return true;
		}
		return false;
	}

	bool checkOutOfBounds(olc::vi2d vec) {
		return vec.x < 0 || vec.x > GAME_GRID_SIZE_X || vec.y < 0 || vec.y > GAME_GRID_SIZE_Y;
	}

	uint8_t getTile(uint8_t x, uint8_t y) {
		if (x < 0 || y < 0 || x > GAME_GRID_SIZE_X || y > GAME_GRID_SIZE_Y) {
			return -1;
		}

		return tiles[(int)x * (int)GAME_GRID_SIZE_Y + (int)y];
	}
};

GameMap initialise_map();

#endif // !GAME_MAP