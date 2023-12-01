#ifndef OLC_LAUNCHER
#define OLC_LAUNCHER

#pragma once

#include <olcPixelGameEngine.h>
#include "GameEngine.h"

class GameEngine;

class olcLauncher : public olc::PixelGameEngine {
private:
	GameEngine* gEngine = nullptr;
	GameSettings* gSettings = nullptr;

public:
	olcLauncher();
	olcLauncher(GameEngine* engine, GameSettings* settings);
	~olcLauncher();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void run();
	virtual bool OnConsoleCommand(const std::string& sCommand);
};

#endif // OLC_LAUNCHER 