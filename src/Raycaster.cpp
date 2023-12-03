#include <math.h> 
#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"
#include "Settings.h"

Raycaster::Raycaster() {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1a.png"});
	dWall = new olc::Decal(sWall);
	
}

Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1.png"});
	dWall = new olc::Decal(sWall);
}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	// Rays could have been updated in the last cycle by console command
	if (fRays != (int)gEngine->gSettings->Camera.Rays) {
		fRays = (int)gEngine->gSettings->Camera.Rays;
		m_rays.clear();
	}

	olc::vi2d mapCoords = gEngine->gPlayer->posMap();

	float fRayAngle = fAngle - gEngine->gSettings->Camera.FovHalf + 0.0001; //radians

	float ox = coords.x, oy = coords.y;
	int mapX = mapCoords.x, mapY = mapCoords.y;
	float step = 1.0f;
	float screenDist = gEngine->gSettings->Window.Width * std::tan(gEngine->gSettings->Camera.FovHalf);
	uint8_t iTextHorz = 1, iTextVert = 1, iText = 1;
	float iTextOff = 0.0f;

	for (int i = 0; i < (int)gEngine->gSettings->Camera.Rays; i++) {
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

		for (int i = 0; i < gEngine->gSettings->Camera.MaxDepth; i++) {
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

		for (int i = 0; i < gEngine->gSettings->Camera.MaxDepth; i++) {
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
		float projHeight = (screenDist / (fDepth + 1e-4)) * gEngine->gSettings->Camera.ProjScale;

		// Insert
		if (m_rays.empty() || i + 1 > m_rays.size()) {
			m_rays.push_back({dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff});
		}
		// Update
		else {
			m_rays[i].update(dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff);
		}

		fRayAngle += gEngine->gSettings->Camera.DeltaAngle;
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

				float dx = coords.x * gEngine->gSettings->Grid.SizeX, dy = coords.y * gEngine->gSettings->Grid.SizeY;
				dx += gEngine->gSettings->Grid.SizeX * ray.depth * ray.dx;
				dy += gEngine->gSettings->Grid.SizeY * ray.depth * ray.dy;

				float sx = coords.x * gEngine->gSettings->Grid.SizeX;
				float sy = coords.y * gEngine->gSettings->Grid.SizeY;
				pge->DrawLineDecal({ sx, sy }, { dx, dy }, gEngine->gSettings->Camera.RayColour);
			}
		}
	}

	if (gEngine->renderMode == GameRenderMode::PROJECTED) {
		int i = 0;
		
		for (auto ray = m_rays.rbegin(); ray != m_rays.rend(); ray++) {
			olc::vf2d wallPos = { 0, 0 }, wallCol = { 0, 0 };

			if (ray->projection < gEngine->gSettings->Window.Height) {
				wallPos = { (float)(i * gEngine->gSettings->Camera.Scale), (float)(gEngine->gSettings->Window.HeightHalf - ray->projection / 2) };
			}
			else {
				wallPos = { (float)(i * gEngine->gSettings->Camera.Scale), 0 };
			}

			olc::vf2d sz{ (float)gEngine->gSettings->Camera.Scale, ray->projection };
			float d = 1 / ray->depth * 3;
			float alpha = (255 * d) / 255;
			olc::Pixel col = olc::WHITE * alpha;

			auto LOD = gEngine->getLod();

			// Render textured walls
			if (LOD.text == 2 || LOD.text == 3) {
				// float scale = (float)T_SIZE / (float)dWall->sprite->width * (gEngine->gSettings->Window.HeightHalf / ray->projection);
				float scale = 1;
				float offX = (float)(i * gEngine->gSettings->Camera.Scale);
				float offY = ray->tOffset;

				const float dw = T_SIZE;

				auto norm = [dw](float s){
					if (s > dw) return dw;
					if (s < 0) return 0.0f;
					return s / dw;
				};
				
				// pge->DrawDecal(wallPos, dWall, {scale, scale});
				pge->DrawPartialDecal(wallPos, dWall, {offX, 0-offY}, {(float)gEngine->gSettings->Camera.Scale, (float)ray->projection}, {scale, scale});
				// pge->DrawPartialDecal(wallPos, dWall, {0, 0}, {(float)gEngine->gSettings->Camera.Scale, (float)ray->projection});
				// std::vector<olc::vf2d> pts{wallPos, {wallPos.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y}};
				// std::vector<olc::vf2d> pts{wallPos, {wallPos.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y}};
				// std::array<olc::vf2d, 4> pos{wallPos, {wallPos.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y + sz.y}, {wallPos.x + sz.x, wallPos.y}};
				// std::vector<olc::vf2d> uv;
				// for (auto p : pts) uv.push_back({norm(p.x), norm(p.y)});

				// pge->DrawPartialWarpedDecal(dWall, pos, {offX, offY}, {(float)gEngine->gSettings->Camera.Scale, (float)ray->projection});
				// pge->DrawPolygonDecal(dWall, pts, uv);
			} else {
				pge->FillRectDecal(wallPos, sz, col);
			}
			i++;
		}
	}
}

std::vector<Ray> Raycaster::getRaysToRender() {
	std::vector<Ray> rays;
	return rays;
}