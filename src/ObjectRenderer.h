#ifndef GAME_OBJECT_RENDERER
#define GAME_OBJECT_RENDERER
#pragma once

#include <olcPixelGameEngine.h>
#include <vector>
#include "Definitions.h"

class GameEngine;

class ObjectRenderer {
private:
	GameEngine* gEngine = nullptr;
	std::vector<std::string> m_wallTexts;

public:
	ObjectRenderer();
	ObjectRenderer(GameEngine* engine);

public:
	void init();
	void getTexture();
};

#endif // GAME_OBJECT_RENDERER