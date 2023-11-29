#pragma once

#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"

Raycaster::Raycaster() {}
Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	olc::vi2d mapCoords = gEngine->gPlayer->posMap();

	float fRayAngle = fAngle - C_HALF_FOV + 0.0001; //radians
	float fRayDeg = RADS_TO_DEGS(fRayAngle);

	float ox = coords.x, oy = coords.y;
	int mapX = mapCoords.x, mapY = mapCoords.y;
	float step = 1.0f;

	for (int i = 0; i < (int)C_NUM_RAYS; i++) {
		float fSin = std::sin(fRayAngle);
		float fCos = std::cos(fRayAngle);
		float dx = 0.0f, dy = 0.0f, fVertX = 0.0f, fVertY = 0.0f, 
			fHorzY = 0.0f, fHorzX = 0.0f, fDepth = 0.0f, 
			fDepthVert = 0.0f, fDepthHorz = 0.0f, fDepthDelta = 0.0f;

		// horizontals
		if (fCos > 0) {
			fHorzY = mapY + step;
			dy = step;
		}
		else {
			fHorzY = mapY - 1e-6;
			dy = -step;
		}

		fDepthHorz = (fHorzY - oy) / fCos;
		fHorzX = ox + fDepthHorz * fSin;
		fDepthDelta = dy / fCos;
		dx = fDepthDelta * fSin;

		for (int i = 0; i < C_MAX_DEPTH; i++) {
			if (gEngine->gMap->checkIntercept({ (int)fHorzX, (int)fHorzY })) {
				break;
			}
			fHorzX += dx;
			fHorzY += dy;
			fDepthHorz += fDepthDelta;
		}

		// Verticals
		if (fSin > 0) {
			fVertX = mapX + step;
			dx = step;
		}
		else {
			fVertX = mapX - 1e-6;
			dx = -step;
		}

		fDepthVert = (fVertX - ox) / fSin;
		fVertY = oy + fDepthVert * fCos;
		fDepthDelta = dx / fSin;
		dy = fDepthDelta * fCos;

		for (int i = 0; i < C_MAX_DEPTH; i++) {
			if (gEngine->gMap->checkIntercept({ (int)fVertX, (int)fVertY })) {
				break;
			}

			fVertX += dx;
			fVertY += dy;
			fDepthVert += fDepthDelta;
		}

		// Depth
		if (fDepthVert < fDepthHorz)
			fDepth = fDepthVert;
		else
			fDepth = fDepthHorz;

		float sx = 0.0f, sy = 0.0f;
		dx = fSin;
		dy = fCos;

		// Insert
		if (m_rays.empty() || i + 1 > m_rays.size()) {
			m_rays.push_back({dx, dy, fRayAngle, fDepth});
		}
		// Update
		else {
			m_rays[i].update(dx, dy, fRayAngle, fDepth);
		}

		fRayAngle += C_DELTA_ANGLE;
		fRayDeg = RADS_TO_DEGS(fRayAngle);
	}
}

void Raycaster::update(float fElapsedTime) {
	castRays(gEngine->gPlayer->posPlayer(), gEngine->gPlayer->playerAngle());
}

void Raycaster::render(olc::PixelGameEngine* pge) {
	// Don't draw rays
	if (!gEngine->bDrawRays)
		return;

	auto coords = gEngine->gPlayer->posPlayer();
	for (auto ray : m_rays) {
		float fSin = std::sin(ray.angle), fCos = std::cos(ray.angle);

		float dx = coords.x * GAME_GRID_PX_SIZE_X, dy = coords.y * GAME_GRID_PX_SIZE_Y;
		dx += GAME_GRID_PX_SIZE_X * ray.magnitude * ray.dx;
		dy += GAME_GRID_PX_SIZE_Y * ray.magnitude * ray.dy;
		pge->DrawLineDecal({ coords.x * GAME_GRID_PX_SIZE_X, coords.y * GAME_GRID_PX_SIZE_Y }, { dx, dy }, olc::YELLOW);
	}
}