#pragma once

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#define GAME_WIDTH 800
#define GAME_HEIGHT 450
#define GAME_PIXEL 2
#define GAME_GRID_SIZE 16
#define GAME_GRID_PX_SIZE_X (GAME_WIDTH / GAME_GRID_SIZE)
#define GAME_GRID_PX_SIZE_Y (GAME_HEIGHT / GAME_GRID_SIZE)

#define FPS 60.0f
#define GAME_TICK 1.0f / FPS

#define ENABLE_DEBUG_MODE false
#define FS_MODE false
#define VSYNC_MODE false
#define PX_COHESION false

#define PLAYER_SPEED 0.004
#define PLAYER_ROT_SPEED 0.002

#endif // !__DEFINITIONS__