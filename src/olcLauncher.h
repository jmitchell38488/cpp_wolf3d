#ifndef OLC_LAUNCHER
#define OLC_LAUNCHER

#pragma once

#include <olcPixelGameEngine.h>
#include "GameEngine.h"

class GameEngine;

class olcLauncher : public olc::PixelGameEngine {
private:
	GameEngine* gEngine = nullptr;

public:
	olcLauncher();
	olcLauncher(GameEngine* engine);
	~olcLauncher();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void run();
};

#endif // OLC_LAUNCHER 