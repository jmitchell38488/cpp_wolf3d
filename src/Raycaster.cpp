#pragma once

#include <math.h> 
#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"
#include "Settings.h"

Raycaster::Raycaster() {}
Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {}

GameSettings gSettings = GameSettings::Get();

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	olc::vi2d mapCoords = gEngine->gPlayer->posMap();

	float fRayAngle = fAngle - gSettings.Camera.FovHalf + 0.0001; //radians

	float ox = coords.x, oy = coords.y;
	int mapX = mapCoords.x, mapY = mapCoords.y;
	float step = 1.0f;
	float screenDist = gSettings.Window.Width * std::tan(gSettings.Camera.FovHalf);
	uint8_t iTextHorz = 1, iTextVert = 1, iText = 1;
	float iTextOff = 0.0f;

	for (int i = 0; i < (int)gSettings.Camera.Rays; i++) {
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

		for (int i = 0; i < gSettings.Camera.MaxDepth; i++) {
			if (gEngine->gMap->checkIntercept({ (int)fHorzX, (int)fHorzY })) {
				iTextHorz = gEngine->gMap->getTile((uint8_t)fHorzX, (uint8_t)fHorzY);
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

		for (int i = 0; i < gSettings.Camera.MaxDepth; i++) {
			if (gEngine->gMap->checkIntercept({ (int)fVertX, (int)fVertY })) {
				iTextVert = gEngine->gMap->getTile((uint8_t)fVertX, (uint8_t)fVertY);
				break;
			}

			fVertX += dx;
			fVertY += dy;
			fDepthVert += fDepthDelta;
		}

		float fract = 0.0f, fractInt = 0.0f;
		// Depth
		if (fDepthVert < fDepthHorz) {
			fDepth = fDepthVert;
			iText = iTextVert;
			fract = modf(fVertY, &fractInt);
			iTextOff = fCos > 0 ? fract : 1 - fract;
		}
		else {
			fDepth = fDepthHorz;
			iText = iTextHorz;
			fract = modf(fHorzX, &fractInt);
			iTextOff = fSin > 0 ? fract : 1 - fract;
		}

		float sx = 0.0f, sy = 0.0f;
		dx = fSin;
		dy = fCos;

		// Remove fishbowl effect
		fDepth *= std::cos(fAngle - fRayAngle);

		// Projection
		float projHeight = (screenDist / (fDepth + 1e-4)) * M_PI * 3/4;

		// Insert
		if (m_rays.empty() || i + 1 > m_rays.size()) {
			m_rays.push_back({dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff});
		}
		// Update
		else {
			m_rays[i].update(dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff);
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

				float dx = coords.x * gSettings.Grid.SizeX, dy = coords.y * gSettings.Grid.SizeY;
				dx += gSettings.Grid.SizeX * ray.depth * ray.dx;
				dy += gSettings.Grid.SizeY * ray.depth * ray.dy;

				float sx = coords.x * gSettings.Grid.SizeX;
				float sy = coords.y * gSettings.Grid.SizeY;
				pge->DrawLineDecal({ sx, sy }, { dx, dy }, gSettings.Camera.RayColour);
			}
		}
	}

	if (gEngine->renderMode == GameRenderMode::PROJECTED) {
		int i = 0;
		for (auto ray = m_rays.rbegin(); ray != m_rays.rend(); ray++) {
			olc::vf2d wallPos = { 0, 0 }, wallCol = { 0, 0 };

			if (ray->projection < gSettings.Window.Height) {
				wallPos = { (float)(i * gSettings.Camera.Scale), (float)(GAME_HEIGHT_H - ray->projection / 2) };
			}
			else {
				wallPos = { (float)(i * gSettings.Camera.Scale), 0 };
			}


			olc::vf2d sz{ (float)gSettings.Camera.Scale, ray->projection };
			float d = 1 / ray->depth * 3;
			float alpha = (255 * d) / 255;
			olc::Pixel col = olc::WHITE * alpha;

			//pge->FillTexturedPolygon(wallPos);

			pge->FillRectDecal(wallPos, sz, col);
			i++;
		}
	}
}

std::vector<Ray> Raycaster::getRaysToRender() {
	std::vector<Ray> rays;
	return rays;
}