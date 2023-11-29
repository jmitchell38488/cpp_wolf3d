#pragma once

#include "Raycaster.h"
#include "Definitions.h"
#include "GameEngine.h"

Raycaster::Raycaster() {}
Raycaster::Raycaster(GameEngine* engine) : gEngine(engine) {}

//void Raycaster::castRays(olc::vf2d coords, float fAngle) {
//	float fDegs = RADS_TO_DEGS(fAngle);
//	float fRayAngle = ROT_RADS(fAngle - C_HALF_FOV + 0.0001); //radians
//	float fRayDeg = RADS_TO_DEGS(fRayAngle);
//	float fDepth = 16.0f;
//
//	for (int i = 0; i < (int)C_NUM_RAYS; i++) {
//		float fStepSize = 0.01f;
//		float fDistance = 0.0f;
//
//		bool bHitWall = false;
//		bool bHitBoundary = false;
//		bool bLit = false;
//
//		float fEyeX = sinf(fRayAngle);
//		float fEyeY = sinf(fRayAngle);
//
//		float fSampleX = 0.0f;
//
//		while (!bHitWall && fDistance < fDepth) {
//			fDistance += fStepSize;
//			int nTestX = (int)(coords.x + fEyeX * fDistance);
//			int nTestY = (int)(coords.y + fEyeY * fDistance);
//
//			if (gEngine->gMap->checkOutOfBounds({ nTestX, nTestY })) {
//				bHitWall = true;
//				fDistance = fDepth;
//				continue;
//			}
//
//			if (gEngine->gMap->checkIntercept({ nTestX, nTestY })) {
//				bHitWall = true;
//				float fBlockMidX = (float)nTestX + 0.5f;
//				float fBlockMidY = (float)nTestY + 0.5f;
//				float fTestPointX = coords.x + fEyeX * fDistance;
//				float fTestPointY = coords.y + fEyeY * fDistance;
//
//				float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));
//
//				if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.25f)
//					fSampleX = fTestPointY - (float)nTestY;
//				if (fTestAngle >= M_PI * 0.25f && fTestAngle < M_PI * 0.75f)
//					fSampleX = fTestPointX - (float)nTestX;
//				if (fTestAngle < M_PI * 0.25f && fTestAngle >= M_PI * 0.75f)
//					fSampleX = fTestPointX - (float)nTestX;
//				if (fTestAngle >= M_PI * 0.75f || fTestAngle < M_PI * 0.75f)
//					fSampleX = fTestPointY - (float)nTestY;
//
//				if (m_rays.empty() || i + 1 > m_rays.size()) {
//					m_rays.push_back({
//						coords.x * GAME_GRID_PX_SIZE_X,
//						coords.y * GAME_GRID_PX_SIZE_Y,
//						fTestPointX * GAME_GRID_PX_SIZE_X,
//						fTestPointY * GAME_GRID_PX_SIZE_Y,
//						fRayAngle
//					});
//				}
//				// Update
//				else {
//					m_rays[i].update(
//						coords.x * GAME_GRID_PX_SIZE_X,
//						coords.y * GAME_GRID_PX_SIZE_Y,
//						fTestPointX * GAME_GRID_PX_SIZE_X,
//						fTestPointY * GAME_GRID_PX_SIZE_Y,
//						fRayAngle
//					);
//				}
//			}
//
//		}
//		fRayAngle = ROT_RADS(fRayAngle + C_DELTA_ANGLE);
//	}
//}

//void Raycaster::castRays(olc::vf2d coords, float fAngle) {
//	olc::vi2d mapCoords { (int)coords.x, (int)coords.y };
//
//	float fDegs = RADS_TO_DEGS(fAngle);
//	float fRayAngle = ROT_RADS(fAngle - C_HALF_FOV + 0.0001); //radians
//	float fRayDeg = RADS_TO_DEGS(fRayAngle);
//	// float fRayAngle = fAngle - C_HALF_FOV + 0.0001;
//	for (int i = 0; i < (int)C_NUM_RAYS; i++) {
//		float fSin = std::sin(fRayAngle);
//		float fCos = std::cos(fRayAngle);
//		float dx = 0.0f, dy = 0.0f, fVertX = 0.0f, fVertY = 0.0f, fHorzY = 0.0f, fHorzX = 0.0f, fDepth = 0.0f, fDepthVert = 0.0f, fDepthHorz = 0.0f, fDepthDelta = 0.0f;
//
//		// horizontals
//		if (fSin > 0) {
//			fHorzY = mapCoords.y + 1;
//			dy = 1;
//		}
//		else {
//			fHorzY = mapCoords.y - 1e-6;
//			dy = -1;
//		}
//
//		fDepthHorz = (fHorzY / coords.y) / fSin;
//		fHorzX = coords.x + fDepthHorz * fCos;
//		fDepthDelta = dy / fSin;
//		dx = fDepthDelta * fCos;
//
//		int iTileX = (int)(mapCoords.x + fHorzX), iTileY = (int)(mapCoords.y + fHorzY);
//		for (int i = 0; i < C_MAX_DEPTH; i++) {
//			// Optimisation to skip checking same tile
//			if (iTileX != (int)fHorzX || iTileY != (int)fHorzY) {
//				if (gEngine->gMap->checkIntercept({ iTileX, iTileY })) {
//					break; // ray has intercepted wall
//				}
//			}
//
//			fHorzX += dx;
//			fHorzY += dy;
//			fDepthHorz += fDepthDelta;
//		}
//
//		// Verticals
//		if (fCos > 0) {
//			fVertX = mapCoords.x + 1;
//			dx = 1;
//		}
//		else {
//			fVertX = mapCoords.x - 1e-6;
//			dx = -1;
//		}
//
//		fDepthVert = (fVertX - coords.x) / fCos;
//		fVertY = coords.y + fDepthVert * fSin;
//
//		fDepthDelta = dx / fCos;
//		dy = fDepthDelta * fSin;
//
//		for (int i = 0; i < C_MAX_DEPTH; i++) {
//			olc::vi2d tile{ (int)fVertX, (int)fVertY };
//			if (gEngine->gMap->checkIntercept(tile)) {
//				break; // ray has intercepted wall
//			}
//			fVertX += dx;
//			fVertY += dy;
//			fDepthVert += fDepthDelta;
//		}
//
//		// Depth
//		fDepth = fDepthVert < fDepthHorz ? fDepthVert : fDepthHorz;
//
//		float sx = 0.0f, sy = 0.0f;
//		sx = coords.x * GAME_GRID_PX_SIZE_X;
//		sy = coords.y * GAME_GRID_PX_SIZE_Y;
//		dx = coords.x * GAME_GRID_PX_SIZE_X + GAME_GRID_PX_SIZE_X * fDepth * fSin;
//		dy = coords.y * GAME_GRID_PX_SIZE_Y + GAME_GRID_PX_SIZE_Y * fDepth * fCos;
//
//		// Insert
//		if (m_rays.empty() || i + 1 > m_rays.size()) {
//			m_rays.push_back({sx, sy, dx, dy, fRayAngle});
//		}
//		// Update
//		else {
//			m_rays[i].update(sx, sy, dx, dy, fAngle);
//		}
//
//		fRayAngle = ROT_RADS(fRayAngle + C_DELTA_ANGLE);
//		fRayDeg = RADS_TO_DEGS(fRayAngle);
//	}
//}

void Raycaster::castRays(olc::vf2d coords, float fAngle) {
	int mapX = int(coords.x), mapY = int(coords.y);
	float planeX = 0.0f, planeY = 0.66f;



	float fDegs = RADS_TO_DEGS(fAngle);
	float fRayAngle = ROT_RADS(fAngle - C_HALF_FOV + 0.0001); //radians
	float fRayDeg = RADS_TO_DEGS(fRayAngle);
	// float fRayAngle = fAngle - C_HALF_FOV + 0.0001;
	//for (int i = 0; i < (int)C_NUM_RAYS; i++) {
	//}
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