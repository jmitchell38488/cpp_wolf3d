#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <vector>

#include "olcPixelGameEngine.h"
#include "Definitions.h"
#include "GameMap.h"
#include "GamePlayer.h"
#include "Raycaster.h"
#include <olcPixelGameEngine.h>

#pragma once

enum class GameRenderMode {
	TOP, PROJECTED
};

class GameEngine {
protected:
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	float fAccumulatedTime;
	std::vector<olc::Pixel> pixels;
	std::unique_ptr<GameMap> gMap = nullptr;
	std::unique_ptr<GamePlayer> gPlayer = nullptr;
	std::unique_ptr<Raycaster> gRaycaster = nullptr;
	olc::PixelGameEngine* pge = nullptr;

public:
	bool bDrawRays = false;
	bool bDrawPlayerVector = false;
	GameRenderMode renderMode = GameRenderMode::TOP;

public:
	GameEngine();

public:
	void handleInput(float fElapsedTime);
	void doGameUpdate(float fElapsedTime);
	bool update(float fElapsedTime);
	void render();
	PlayerMovDir getPlayerMoveDir();

public:
	void resetGame();
	bool initialise(olc::PixelGameEngine* engine);
	float getRunTime();

private:
	friend class GamePlayer;
	friend class Raycaster;
};

#endif // GAME_ENGINE