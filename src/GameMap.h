#ifndef GAME_MAP
#define GAME_MAP
#pragma once

#include <vector>
#include <olcPixelGameEngine.h>

struct GameMap {
	std::vector<uint16_t> tiles;
	std::vector<olc::vf2d> coords;
};

GameMap initialise_map();

#endif // !GAME_MAP