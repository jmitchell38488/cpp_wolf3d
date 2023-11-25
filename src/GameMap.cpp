#include "GameMap.h"
#include "Definitions.h"

// 0 - empty tile
// 1 - filled wall tile

GameMap initialise_map() {
	GameMap gMap;
	const uint8_t grid = GAME_GRID_SIZE;

	for (uint8_t x = 0; x < grid; x++) {
		for (uint8_t y = 0; y < grid; y++) {
			gMap.tiles.push_back(0);
			if (x == 0) gMap.tiles[x * grid + y] = 1; //top row
			if (y == 0 || y == grid - 1) gMap.tiles[x * grid + y] = 1; //sides
			if (x == grid - 1) gMap.tiles[x * grid + y] = 1; //bottom row
		}
	}

	gMap.tiles[3 * grid + 2] = 1;
	gMap.tiles[4 * grid + 2] = 1;
	gMap.tiles[5 * grid + 2] = 1;
	gMap.tiles[6 * grid + 2] = 1;
	gMap.tiles[6 * grid + 3] = 1;
	gMap.tiles[6 * grid + 4] = 1;
	gMap.tiles[6 * grid + 5] = 1;
	gMap.tiles[5 * grid + 5] = 1;
	gMap.tiles[4 * grid + 5] = 1;
	gMap.tiles[3 * grid + 5] = 1;

	gMap.tiles[3 * grid + 13] = 1;
	gMap.tiles[3 * grid + 12] = 1;
	gMap.tiles[3 * grid + 11] = 1;
	gMap.tiles[3 * grid + 10] = 1;
	gMap.tiles[4 * grid + 10] = 1;
	gMap.tiles[5 * grid + 10] = 1;
	gMap.tiles[6 * grid + 10] = 1;
	gMap.tiles[6 * grid + 11] = 1;

	gMap.tiles[9 * grid + 14] = 1;
	gMap.tiles[12 * grid + 14] = 1;

	gMap.tiles[9 * grid + 4] = 1;
	gMap.tiles[10 * grid + 4] = 1;
	gMap.tiles[11 * grid + 4] = 1;
	gMap.tiles[11 * grid + 5] = 1;
	gMap.tiles[11 * grid + 6] = 1;
	gMap.tiles[11 * grid + 7] = 1;
	gMap.tiles[11 * grid + 8] = 1;
	gMap.tiles[12 * grid + 8] = 1;
	gMap.tiles[13 * grid + 8] = 1;

	gMap.tiles[14 * grid + 12] = 1;

	for (uint8_t x = 0; x < GAME_GRID_SIZE; x++) {
		for (uint8_t y = 0; y < GAME_GRID_SIZE; y++) {
			if (gMap.tiles[x * GAME_GRID_SIZE + y] == 1)
				gMap.coords.push_back({ (float)x, (float)y });
		}
	}

	return gMap;
}
