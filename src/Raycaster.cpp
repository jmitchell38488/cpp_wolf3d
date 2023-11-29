#pragma once

#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"

Raycaster::Raycaster() {}
Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	olc::vi2d mapCoords { (int)coords.x, (int)coords.y };

	float fRayAngle = fAngle - C_HALF_FOV + 0.0001;
	for (int i = 0; i < (int)C_NUM_RAYS; i++) {
		float fSin = std::sin(fRayAngle);
		float fCos = std::cos(fRayAngle);

		float dx = 0.0f, dy = 0.0f, fVertX = 0.0f, fVertY = 0.0f, fHorzY = 0.0f, fHorzX = 0.0f, fDepth = 0.0f, fDepthVert = 0.0f, fDepthHorz = 0.0f, fDepthDelta = 0.0f;

		// horizontals
		if (fSin > 0) {
			fHorzY = mapCoords.y + 1;
			dy = 1;
		}
		else {
			fHorzY = mapCoords.y - 1e6;
			dy = -1;
		}

		fDepthHorz = (fHorzY / coords.y) / fSin;
		fHorzX = coords.x + fDepthHorz * fCos;
		fDepthDelta = dy / fSin;
		dx = fDepthDelta * fCos;

		for (int i = 0; i < C_MAX_DEPTH; i++) {
			olc::vi2d tile{ (int)fHorzX, (int)fHorzY };
			if (gEngine->gMap->checkIntercept(tile)) {
				break; // ray has intercepted wall
			}
			fHorzX += dx;
			fHorzY += dy;
			fDepthHorz += fDepthDelta;
		}

		// Verticals
		if (fCos > 0) {
			fVertX = mapCoords.x + 1;
			dx = 1;
		}
		else {
			fVertX = mapCoords.x - 1e-6;
			dx = -1;
		}

		fDepthVert = (fVertX - coords.x) / fCos;
		fVertY = coords.y + fDepthVert * fSin;

		fDepthDelta = dx / fCos;
		dy = fDepthDelta * fSin;

		for (int i = 0; i < C_MAX_DEPTH; i++) {
			olc::vi2d tile{ (int)fVertX, (int)fVertY };
			if (gEngine->gMap->checkIntercept(tile)) {
				break; // ray has intercepted wall
			}
			fVertX += dx;
			fVertY += dy;
			fDepthVert += fDepthDelta;
		}

		// Depth
		fDepth = fDepthVert < fDepthHorz ? fDepthVert : fDepthHorz;
		// Insert
		if (m_rays.empty() || i + 1 > m_rays.size()) {
			m_rays.push_back({
				coords.x * GAME_GRID_PX_SIZE_X, //sx
				coords.y * GAME_GRID_PX_SIZE_Y, //sy
				coords.x * GAME_GRID_PX_SIZE_Y + GAME_GRID_PX_SIZE_Y * fDepth * fSin, //dx
				coords.y * GAME_GRID_PX_SIZE_Y + GAME_GRID_PX_SIZE_Y * fDepth * fCos, //dy
				fRayAngle
				});
		}
		// Update
		else {
			Ray * r = &m_rays[i];
			r->sx = coords.x * GAME_GRID_PX_SIZE_X;
			r->sy = coords.y * GAME_GRID_PX_SIZE_Y;
			r->dx = coords.x * GAME_GRID_PX_SIZE_Y + GAME_GRID_PX_SIZE_Y * fDepth * fSin;
			r->dy = coords.y * GAME_GRID_PX_SIZE_Y + GAME_GRID_PX_SIZE_Y * fDepth * fCos;
			r->angle = fRayAngle;
		}

		fRayAngle += C_DELTA_ANGLE;
	}
}

void Raycaster::update(float fElapsedTime) {
	castRays(gEngine->gPlayer->posPlayer(), gEngine->gPlayer->playerAngle());
}

void Raycaster::render(olc::PixelGameEngine* pge) {
	// Don't draw rays
	if (!gEngine->bDrawRays)
		return;

	for (auto ray : m_rays) {
		pge->DrawLineDecal({ ray.sx, ray.sy }, { ray.dx, ray.dy }, olc::YELLOW);
	}
}