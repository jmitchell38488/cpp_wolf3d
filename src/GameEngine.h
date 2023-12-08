#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <vector>
#include <olcPixelGameEngine.h>

#include "GameMap.h"
#include "GamePlayer.h"
#include "Raycaster.h"
#include "ObjectRenderer.h"
#include "Settings.h"


#pragma once

enum class GameRenderMode {
	TOP, PROJECTED
};

class GameEngine {
private:
	struct LOD {
		uint16_t rays; // ray casting quality
		uint8_t text; // texture mapping {1,2,3}: 1-flat, 2-lores, 3-hires
		uint8_t ddist; // draw distance
		uint8_t llevel; // light levels {1,2,3}: 1-low, 2-med, 3-high
	};

	std::vector<LOD> lods{
		{100, 1, 15, 1}, // lowest
		{155, 1, 18, 1}, // low
		{210, 2, 20, 2}, // med
		{325, 3, 20, 3}, // high
		{500, 3, 25, 3} // highest
	};

protected:
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	float fAccumulatedTime;
	std::vector<olc::Pixel> pixels;
	std::unique_ptr<GameMap> gMap = nullptr;
	std::unique_ptr<GamePlayer> gPlayer = nullptr;
	std::unique_ptr<Raycaster> gRaycaster = nullptr;
	std::unique_ptr<ObjectRenderer> gObjRenderer = nullptr;
	olc::PixelGameEngine* pge = nullptr;
	GameSettings* gSettings = nullptr;
	uint8_t lod;

public:
	bool bDrawRays = false;
	bool bDrawPlayerVector = false;
	GameRenderMode renderMode = GameRenderMode::PROJECTED;

public:
	GameEngine();
	GameEngine(GameSettings* settings);

public:
	void handleInput(float fElapsedTime);
	void doGameUpdate(float fElapsedTime);
	bool update(float fElapsedTime);
	void render();
	PlayerMovDir getPlayerMoveDir();
	bool OnConsoleCommand(const std::string& sCommand);
	LOD getLod();
	void setLod(uint8_t l);

public:
	void resetGame();
	bool initialise(olc::PixelGameEngine* engine);
	float getRunTime();

private:
	friend class GamePlayer;
	friend class Raycaster;
	friend class ObjectRenderer;
};

#endif // GAME_ENGINE