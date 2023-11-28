#include "GameMap.h"
#include "Definitions.h"

// 0 - empty tile
// 1 - filled wall tile

GameMap initialise_map() {
	GameMap gMap;
	const uint8_t gridX = GAME_GRID_SIZE_X;
	const uint8_t gridY = GAME_GRID_SIZE_Y;

	for (uint8_t x = 0; x < gridX; x++) {
		for (uint8_t y = 0; y < gridY; y++) {
			gMap.tiles.push_back(0);
			if (x == 0 || x == gridX - 1 || y == 0 || y == gridY - 1)
				gMap.tiles[x * gridY + y] = 1;
		}
	}

	// x * y.size + y
	gMap.tiles[3 * gridY + 2] = 1;
	gMap.tiles[4 * gridY + 2] = 1;
	gMap.tiles[5 * gridY + 2] = 1;
	gMap.tiles[6 * gridY + 2] = 1;
	gMap.tiles[6 * gridY + 3] = 1;
	gMap.tiles[6 * gridY + 4] = 1;
	gMap.tiles[6 * gridY + 5] = 1;
	gMap.tiles[5 * gridY + 5] = 1;
	gMap.tiles[4 * gridY + 5] = 1;
	gMap.tiles[3 * gridY + 5] = 1;

	gMap.tiles[9 * gridY + 3] = 1;
	gMap.tiles[10 * gridY + 3] = 1;
	gMap.tiles[11 * gridY + 3] = 1;
	gMap.tiles[11 * gridY + 4] = 1;
	gMap.tiles[11 * gridY + 5] = 1;

	gMap.tiles[3 * gridY + 7] = 1;
	gMap.tiles[7 * gridY + 7] = 1;

	/*
	gMap.tiles[3 * gridY + 13] = 1;
	gMap.tiles[3 * gridY + 12] = 1;
	gMap.tiles[3 * gridY + 11] = 1;
	gMap.tiles[3 * gridY + 10] = 1;
	gMap.tiles[4 * gridY + 10] = 1;
	gMap.tiles[5 * gridY + 10] = 1;
	gMap.tiles[6 * gridY + 10] = 1;
	gMap.tiles[6 * gridY + 11] = 1;

	gMap.tiles[9 * gridY + 14] = 1;
	gMap.tiles[12 * gridY + 14] = 1;

	gMap.tiles[11 * gridY + 7] = 1;
	gMap.tiles[11 * gridY + 8] = 1;
	gMap.tiles[12 * gridY + 8] = 1;
	gMap.tiles[13 * gridY + 8] = 1;

	gMap.tiles[14 * gridY + 12] = 1;
	*/

	for (uint8_t x = 0; x < gridX; x++) {
		for (uint8_t y = 0; y < gridY; y++) {
			if (gMap.tiles[x * gridY + y] == 1)
				gMap.coords.push_back({ (float)x, (float)y });
		}
	}

	return gMap;
}
