#ifndef GAME_SETTINGS
#define GAME_SETTINGS

#pragma once

#include "Definitions.h"
#include <olcPixelGameEngine.h>

class GameSettings {

private:
	struct GS_Window {
		double Width = GAME_WIDTH;
		double Height = GAME_HEIGHT;
		double WidthHalf = GAME_WIDTH_H;
		double HeightHalf = GAME_HEIGHT_H;
		double Pixel = GAME_PIXEL;
		bool FullScreen = FS_MODE;
		bool VSync = VSYNC_MODE;
		bool PxCoh = PX_COHESION;
	};

	struct GS_Grid {
		int CountX = GAME_GRID_SIZE_X;
		int CountY = GAME_GRID_SIZE_Y;
		double SizeX = GAME_GRID_PX_SIZE_X;
		double SizeY = GAME_GRID_PX_SIZE_Y;
	};

	struct GS_Player {
		double Speed = PLAYER_SPEED;
		double Rotate = PLAYER_ROT_SPEED;
		double Sens = MOUSE_SENS;
		double WlkSpeed = PLAYER_SPEED / 2;
	};

	struct GS_Game {
		double Fps = FPS;
		double Ticks = GAME_TICK;
		bool DebugMode = ENABLE_DEBUG_MODE;
	};

	struct GS_Camera {
		double Fov = C_FOV;
		double FovHalf = C_HALF_FOV;
		double Rays = C_NUM_RAYS;
		double DeltaAngle = C_DELTA_ANGLE;
		double MaxDepth = C_MAX_DEPTH;
		double Scale = C_SCALE;
		double ProjScale = M_PI / 2;
		olc::Pixel RayColour = { 255, 255, 0, 255 };
	};

private:
	bool bInitialized;

public:
	GS_Window Window;
	GS_Grid Grid;
	GS_Player Player;
	GS_Game Game;
	GS_Camera Camera;

public:
	GameSettings();

public:
	void init();

};

#endif // GAME_SETTINGS