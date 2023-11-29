#pragma once

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524401
#endif // M_SQRT1_2

#ifndef M_RAD_DEG
#define M_RAD_DEG (180 / M_PI)
#endif // M_RAD_DEG

#ifndef M_DEG_RAD
#define M_DEG_RAD (M_PI / 180)
#endif // M_DEG_RAD

#ifndef M_RAD_DEGS_360
#define M_RAD_DEGS_360 360 * M_DEG_RAD
#endif // M_RAD_DEGS_360

#define GAME_WIDTH 800
#define GAME_HEIGHT 450
#define GAME_PIXEL 1
#define GAME_GRID_SIZE_X 16
#define GAME_GRID_SIZE_Y 9
#define GAME_GRID_PX_SIZE_X (GAME_WIDTH / GAME_GRID_SIZE_X)
#define GAME_GRID_PX_SIZE_Y (GAME_HEIGHT / GAME_GRID_SIZE_Y)

#define FPS 60.0f
#define GAME_TICK 1.0f / FPS

#define ENABLE_DEBUG_MODE false
#define FS_MODE false
#define VSYNC_MODE false
#define PX_COHESION false

#define PLAYER_SPEED 2.75
#define PLAYER_ROT_SPEED 1.85
#define MOUSE_SENS 1.0

// Camera
#define C_FOV (M_PI / 3)
#define C_HALF_FOV (C_FOV / 2)
#define C_NUM_RAYS (GAME_WIDTH / 2)
#define C_DELTA_ANGLE (C_FOV / C_NUM_RAYS)
#define C_MAX_DEPTH 20

#endif // !__DEFINITIONS__