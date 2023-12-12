#ifndef GAME_RAYCASTER_STEP
#define GAME_RAYCASTER_STEP

#include <olcPixelGameEngine.h>
#include <vector>
#include "Raycaster.h"

class GameEngine;
class Raycaster;

class Raycaster_Step : public Raycaster {
public:
	Raycaster_Step(GameEngine* engine);

public:
	void render(olc::PixelGameEngine* pge);
	void update(float fElapsedTime);

protected:
	void castRays(olc::vf2d coords, float fAngle);
	std::array<olc::vf2d, 2> getProjectionCoords(Ray * ray, int offset);
};

#endif // GAME_RAYCASTER_STEP
