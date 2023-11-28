#ifndef GAME_MAP
#define GAME_MAP
#pragma once

#include <vector>
#include <olcPixelGameEngine.h>

struct GameMap {
	std::vector<uint16_t> tiles;
	std::vector<olc::vf2d> coords;

	bool checkIntercept(olc::vi2d vec) {
		for (auto coord : coords) {
			if (vec.x == coord.x && vec.y == coord.y)
				return true;
		}
		return false;
	}
};

GameMap initialise_map();

#endif // !GAME_MAP