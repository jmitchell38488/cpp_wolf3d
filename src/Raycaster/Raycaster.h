#ifndef GAME_RAYCASTER
#define GAME_RAYCASTER

#include <olcPixelGameEngine.h>
#include <vector>

class GameEngine;

enum class RayFace {
	TOP, BOTTOM, LEFT, RIGHT
};

struct Ray {
	float dx;
	float dy;
	float angle;
	float depth;
	float projection;
	uint8_t texture;
	float tOffset;
	olc::vi2d tile;
	RayFace face;

	void update(float px, float py, float ang, float dep, float proj, uint8_t text, float off, olc::vi2d t, RayFace f) {
		dx = px;
		dy = py;
		angle = ang;
		depth = dep;
		projection = proj;
		texture = text;
		tOffset = off;
		tile = t;
		face = f;
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

protected:
	void castRays(olc::vf2d coords, float fAngle);
	bool adjacentTiles(olc::vi2d c1, olc::vi2d c2);
	std::array<olc::vf2d, 4> getQuadVertices(olc::vf2d dm, olc::vf2d sm);
	std::array<olc::vf2d, 2> getProjectionCoords(Ray * ray, int offset);

protected:
	float fRays = 0.0f;
	olc::Sprite * sWall = nullptr;
	olc::Decal * dWall = nullptr;
	GameEngine* gEngine = nullptr;
	std::vector<Ray> m_rays;
};

#endif // GAME_RAYCASTER
