#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <vector>

#include "olcPixelGameEngine.h"
#include "Definitions.h"
#include "GameMap.h"
#include "GamePlayer.h"
#include <olcPixelGameEngine.h>

#pragma once

class GameEngine {
protected:
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	float fAccumulatedTime;
	std::vector<olc::Pixel> pixels;
	std::unique_ptr<GameMap> gMap = nullptr;
	GamePlayer* gPlayer;

public:
	GameEngine();

public:
	void handleInput(float fElapsedTime, olc::PixelGameEngine* pge);
	void doGameUpdate(float fElapsedTime, olc::PixelGameEngine* pge);
	bool update(float fElapsedTime, olc::PixelGameEngine * pge);
	void render(olc::PixelGameEngine* pge);

public:
	void resetGame();
	bool initialise();
	float getRunTime();
};

#endif // GAME_ENGINE