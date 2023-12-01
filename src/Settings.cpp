#include "Settings.h"
#include "Definitions.h"
#include <iostream>
#include <string.h>

using namespace std;

GameSettings::GameSettings() {
	bInitialized = false;
}

void GameSettings::init() {
	cout << "Initializing GameSettings" << endl;
	if (bInitialized) return;
	bInitialized = true;

	GS_Window gs_win{ GAME_WIDTH , GAME_HEIGHT ,GAME_WIDTH_H ,GAME_HEIGHT_H , GAME_PIXEL, FS_MODE, VSYNC_MODE , PX_COHESION };
	Window = gs_win;

	GS_Grid gs_grid{ GAME_GRID_SIZE_X , GAME_GRID_SIZE_Y , GAME_GRID_PX_SIZE_X , GAME_GRID_PX_SIZE_Y };
	Grid = gs_grid;

	GS_Player gs_player{ PLAYER_SPEED , PLAYER_ROT_SPEED , MOUSE_SENS };
	Player = gs_player;

	GS_Game gs_game{ FPS , GAME_TICK , ENABLE_DEBUG_MODE };
	Game = gs_game;

	GS_Camera gs_camera{ C_FOV , C_HALF_FOV , C_NUM_RAYS , C_DELTA_ANGLE , C_MAX_DEPTH , C_SCALE, M_PI / 2, olc::YELLOW };
	Camera = gs_camera;
}