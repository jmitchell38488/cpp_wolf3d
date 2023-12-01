#include "Settings.h"
#include "Definitions.h"

GameSettings::GameSettings() {
	bInitialized = false;
}

void GameSettings::init() {
	if (bInitialized) return;
	bInitialized = true;

	GameSettings gSettings;

	GS_Window Window{ GAME_WIDTH , GAME_HEIGHT ,GAME_WIDTH_H ,GAME_HEIGHT_H , GAME_PIXEL, FS_MODE, VSYNC_MODE , PX_COHESION };
	gSettings.Window = Window;

	GS_Grid Grid{ GAME_GRID_SIZE_X , GAME_GRID_SIZE_Y , GAME_GRID_PX_SIZE_X , GAME_GRID_PX_SIZE_Y };
	gSettings.Grid = Grid;

	GS_Player Player{ PLAYER_SPEED , PLAYER_ROT_SPEED , MOUSE_SENS };
	gSettings.Player = Player;

	GS_Game Game{ FPS , GAME_TICK , ENABLE_DEBUG_MODE };
	gSettings.Game = Game;

	GS_Camera Camera{ C_FOV , C_HALF_FOV , C_NUM_RAYS , C_DELTA_ANGLE , C_MAX_DEPTH , C_SCALE, olc::YELLOW };
	gSettings.Camera = Camera;
}