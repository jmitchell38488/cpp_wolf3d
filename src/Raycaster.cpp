#pragma once

#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"

Raycaster::Raycaster() {}
Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	olc::vi2d mapCoords = gEngine->gPlayer->posMap();

	float fRayAngle = fAngle - C_HALF_FOV + 0.0001; //radians

	float ox = coords.x, oy = coords.y;
	int mapX = mapCoords.x, mapY = mapCoords.y;
	float step = 1.0f;
	float screenDist = GAME_WIDTH_H * std::tan(C_HALF_FOV);

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

		// Remove fishbowl effect
		fDepth *= std::cos(fAngle - fRayAngle);

		// Projection
		float projHeight = screenDist / (fDepth + 1e-4);

		// Insert
		if (m_rays.empty() || i + 1 > m_rays.size()) {
			m_rays.push_back({dx, dy, fRayAngle, fDepth, projHeight});
		}
		// Update
		else {
			m_rays[i].update(dx, dy, fRayAngle, fDepth, projHeight);
		}

		fRayAngle += C_DELTA_ANGLE;
	}
}

void Raycaster::update(float fElapsedTime) {
	castRays(gEngine->gPlayer->posPlayer(), gEngine->gPlayer->playerAngle());
}

void Raycaster::render(olc::PixelGameEngine* pge) {
	auto coords = gEngine->gPlayer->posPlayer();
	if (gEngine->renderMode == GameRenderMode::TOP) {
		// Render rays
		if (gEngine->bDrawRays) {
			for (auto ray : m_rays) {
				float fSin = std::sin(ray.angle), fCos = std::cos(ray.angle);

				float dx = coords.x * GAME_GRID_PX_SIZE_X, dy = coords.y * GAME_GRID_PX_SIZE_Y;
				dx += GAME_GRID_PX_SIZE_X * ray.depth * ray.dx;
				dy += GAME_GRID_PX_SIZE_Y * ray.depth * ray.dy;
				pge->DrawLineDecal({ coords.x * GAME_GRID_PX_SIZE_X, coords.y * GAME_GRID_PX_SIZE_Y }, { dx, dy }, olc::YELLOW);
			}
		}
	}

	if (gEngine->renderMode == GameRenderMode::PROJECTED) {
		for (int i = 0; i < m_rays.size(); i++) {
			const auto ray = &m_rays[i];
			olc::vf2d wallPos = { 0, 0 }, wallCol = { 0, 0 };

			if (ray->projection < GAME_HEIGHT) {
				wallPos = { (float)(i * C_SCALE), (float)(GAME_HEIGHT_H - ray->projection / 2) };
			}
			else {
				wallPos = { (float)(i * C_SCALE), 0 };
			}

			olc::vf2d sz{ C_SCALE, ray->projection };
			pge->FillRectDecal(wallPos, sz, olc::WHITE);
		}
	}
}