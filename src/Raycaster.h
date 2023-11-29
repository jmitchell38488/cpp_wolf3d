#ifndef GAME_RAYCASTER
#define GAME_RAYCASTER
#pragma once

#include <olcPixelGameEngine.h>
#include <vector>

class GameEngine;

struct Ray {
	float dx;
	float dy;
	float angle;
	float magnitude;

	void update(float px, float py, float ang, float mag) {
		dx = px;
		dy = py;
		angle = ang;
		magnitude = mag;
	}
};

class Raycaster {
public:
	Raycaster();
	Raycaster(GameEngine* engine);

public:
	void update(float fElapsedTime);
	void castRays(olc::vf2d coords, float fAngle);
	void render(olc::PixelGameEngine* pge);

private:
	GameEngine* gEngine = nullptr;
	std::vector<Ray> m_rays;
};

#endif // GAME_RAYCASTER
