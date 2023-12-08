#include <math.h> 
#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"
#include "Settings.h"

Raycaster::Raycaster() {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1.jpg"});
	dWall = new olc::Decal(sWall);
	
}

Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {
	sWall = new olc::Sprite({(std::string) "./data/resources/textures/1.jpg"});
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
		int mx = 0, my = 0;

		// Degrees: 0 -> right, 90 -> bottom, 180 -> left, 270 -> top
		// From 180 -> 360 (0), can only hit bottom faces
		// From 0 -> 180, can only hit top faces
		// From 270 -> 90, can only hit left faces
		// From 90 -> 270 can only hit right faces

		// Depth
		auto fn = [fRayAngle](bool hz, float fX, float fY) {
			float frX, frY;
			modf(fX, &frX);
			modf(fY, &frY);

			// Hit a corner, exactly {0,0}, we'll pick a face based on angle
			if (frX == frY && frX == 0) {
				if (fRayAngle >= 0 && fRayAngle < 90) return RayFace::RIGHT;
				if (fRayAngle >= 90 && fRayAngle < 180) return RayFace::BOTTOM;
				if (fRayAngle >= 180 && fRayAngle < 270) return RayFace::LEFT;
				if (fRayAngle >= 270) return RayFace::TOP;
			}

			if (frX > 0) { // impacted on x axis
				if (fRayAngle >= 0 && fRayAngle <= 180) return RayFace::TOP;
				if (fRayAngle > 180) return RayFace::BOTTOM;
			}

			if (frY > 0) { // impacted on y axis
				if (fRayAngle > 270 || fRayAngle < 90) return RayFace::LEFT;
				if (fRayAngle >= 90 || fRayAngle <= 270) return RayFace::RIGHT;
			}
		};

		RayFace rFace;
		if (fDepthVert < fDepthHorz) {
			fDepth = fDepthVert;
			iText = iTextVert;
			fract = modf(fVertY, &fractInt);
			iTextOff = fCos > 0 ? fract : 1 - fract;
			mx = (int)fVertX;
			my = (int)fVertY;
			rFace = fn(false, fVertX, fVertY);
		}
		else {
			fDepth = fDepthHorz;
			iText = iTextHorz;
			fract = modf(fHorzX, &fractInt);
			iTextOff = fSin > 0 ? fract : 1 - fract;
			mx = (int)fHorzX;
			my = (int)fHorzY;
			rFace = fn(false, fHorzX, fHorzY);
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
			m_rays.push_back({dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff, {mx, my}, rFace});
		}
		// Update
		else {
			m_rays[i].update(dx, dy, fRayAngle, fDepth, projHeight, iText, iTextOff, {mx, my}, rFace);
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
		std::array<olc::vf2d, 4> pPoints, nPoints, dPoints;

		for (int i = 0; i < m_rays.size(); i++) {
			int idx = m_rays.size() - (i+1);
			auto * ray = &m_rays[idx];
			auto coords = getProjectionCoords(ray, i);
			auto wallPos = coords[0], sz = coords[1];
			std::array<olc::Pixel, 4> cols = { { olc::RED, olc::BLUE, olc::GREEN, olc::WHITE } };

			dPoints = getQuadVertices(wallPos, sz);

			if (i > 0) {
				// Adjust left vertices if wall on the same plane and adjacent tiles
				auto * pRay = &m_rays[idx+1];
				if (pRay->face == ray->face && adjacentTiles(ray->tile, pRay->tile)) {
					dPoints[0].y = pPoints[0].y; // bottom left
					dPoints[3].y = pPoints[3].y; // top left
				}

				// Check next ray
				if (i < m_rays.size() - 2) {
					auto * nRay = &m_rays[idx-1];

					auto nc = getProjectionCoords(nRay, i+1);
					nPoints = getQuadVertices(nc[0], nc[1]);

					// it's a hanging left edge, we need to smooth the left points by the next ray
					if (pRay->face != ray->face || !adjacentTiles(ray->tile, pRay->tile)) {	
						if (ray->face == nRay->face && adjacentTiles(ray->tile, nRay->tile)) {
							auto d0 = dPoints[0].diff(nPoints[0]), d3 = dPoints[3].diff(nPoints[3]);
							if (d0.y != 0) dPoints[0].y += d0.y;
							if (d3.y != 0) dPoints[3].y += d3.y;
						}
					}

					// it's a right edge probably into a corner, we need to smooth the right points by avg of the previous ray
					if (ray->face != nRay->face || !adjacentTiles(ray->tile, nRay->tile)) {
						if (ray->face == pRay->face && adjacentTiles(ray->tile, pRay->tile)) {
							auto d1 = dPoints[1].d_avg(pPoints[1]), d2 = dPoints[2].d_avg(pPoints[2]);
							if (d1.y != 0) dPoints[1].y += d1.y;
							if (d2.y != 0) dPoints[2].y += d2.y;
						}
					}
				}
			}

			pPoints = getQuadVertices(wallPos, sz);
			
			float d = 1 / ray->depth * 3;
			float alpha = (255 * d) / 255;
			olc::Pixel col = olc::WHITE * alpha;

			auto LOD = gEngine->getLod();

			// Render textured walls
			if (LOD.text == 2 || LOD.text == 3) {
				float scale = 1;
				float offX = (float)(i * gEngine->gSettings->Camera.Scale);
				float offY = ray->tOffset;

				const float dw = T_SIZE;

				auto norm = [dw](float s){
					if (s > dw) return dw;
					if (s < 0) return 0.0f;
					return s / dw;
				};
				
				pge->DrawPartialDecal(wallPos, dWall, {offX, 0-offY}, {(float)gEngine->gSettings->Camera.Scale, (float)ray->projection}, {scale, scale});
			} else {
				pge->FillPolygonDecal(dPoints, cols);
				// pge->FillPolygonDecal(dPoints, col);
				// pge->FillRectDecal(wallPos, sz, col);
			}
		}
	}
}

std::vector<Ray> Raycaster::getRaysToRender() {
	std::vector<Ray> rays;
	return rays;
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
	olc::vf2d wallPos = { 0, 0 }, wallCol = { 0, 0 };
	olc::vf2d sz{ (float)gEngine->gSettings->Camera.Scale, ray->projection };

	if (ray->projection < gEngine->gSettings->Window.Height) {
		wallPos = { (float)(offset * gEngine->gSettings->Camera.Scale), (float)(gEngine->gSettings->Window.HeightHalf - ray->projection / 2) };
	}
	else {
		wallPos = { (float)(offset * gEngine->gSettings->Camera.Scale), 0 };
	}

	return std::array<olc::vf2d, 2>{{ wallPos, sz }};
}

bool Raycaster::adjacentTiles(olc::vi2d c1, olc::vi2d c2) {
	olc::vi2d d = c1.diff_a(c2);
	if (d.x > 1 || d.y > 1) return false;
	return true;
}