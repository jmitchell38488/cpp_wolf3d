#include <math.h> 
#include "Raycaster.h"
#include "../Definitions.h"
#include "../GameEngine.h"
#include "../Settings.h"

class GameEngine;

Raycaster::Raycaster() {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1.png"});
	dWall = new olc::Decal(sWall);
}

Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1.png"});
	dWall = new olc::Decal(sWall);
}

void Raycaster::update(float fElapsedTime) {
	// castRays(gEngine->gPlayer->posPlayer(), gEngine->gPlayer->playerAngle());
}

void Raycaster::render(olc::PixelGameEngine* pge) {
  // Do nothing
}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
  // Do nothing
}

std::vector<Ray> Raycaster::getRaysToRender() {
	std::vector<Ray> rays;
	return rays;
}

bool Raycaster::adjacentTiles(olc::vi2d c1, olc::vi2d c2) {
	olc::vi2d d = c1.diff_a(c2);
	if (d.x > 1 || d.y > 1) return false;
	return true;
}

std::array<olc::vf2d, 4> Raycaster::getQuadVertices(olc::vf2d dm, olc::vf2d sm) {
	std::array<olc::vf2d, 4> verts = { {
		{ dm.x, dm.y + sm.y }, // bottom left
		{ dm.x + sm.x, dm.y + sm.y }, // bottom right
		{ dm.x + sm.x, dm.y }, // top right
		{ dm.x, dm.y } // top left
	} };

	return verts;
}

std::array<olc::vf2d, 2> Raycaster::getProjectionCoords(Ray * ray, int offset) {
  return {{{0.0f,0.0f}, {0.0f,0.0f}}};
}