#ifndef GAME_RAYCASTER
#define GAME_RAYCASTER

#include <olcPixelGameEngine.h>
#include <vector>

class GameEngine;

struct Ray {
	float dx;
	float dy;
	float angle;
	float depth;
	float projection;
	uint8_t texture;
	float tOffset;

	void update(float px, float py, float ang, float dep, float proj, uint8_t text, float off) {
		dx = px;
		dy = py;
		angle = ang;
		depth = dep;
		projection = proj;
		texture = text;
		tOffset = off;
	}
};

class Raycaster {
public:
	Raycaster();
	Raycaster(GameEngine* engine);

public:
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* pge);
	std::vector<Ray> getRaysToRender();

private:
	void castRays(olc::vf2d coords, float fAngle);
	float fRays = 0.0f;

private:
	olc::Sprite * sWall = nullptr;
	olc::Decal * dWall = nullptr;
	GameEngine* gEngine = nullptr;
	std::vector<Ray> m_rays;
};

#endif // GAME_RAYCASTER
