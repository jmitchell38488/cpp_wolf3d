#include "GamePlayer.h"
#include "Definitions.h"
#include "GameEngine.h"
#include <olcPixelGameEngine.h>

GamePlayer::GamePlayer() {
	coords = { 1.5, 5 };
	fAngle = 0.0f;
	vDir = { std::sin(fAngle), std::cos(fAngle) };
}

GamePlayer::GamePlayer(GameEngine * engine) : gEngine(engine) {
	coords = { 1.5, 5 };
	fAngle = M_RAD_DEGS_360 * 0.25;
	vDir = { std::sin(fAngle), std::cos(fAngle) };
}

void GamePlayer::movement(float fElapsedTime) {
	// No movement to calculate
	if (pMove == PlayerMovDir::NONE) {
		return;
	}

	float fSin = std::sin(fAngle);
	float fCos = std::cos(fAngle);
	float fSin2 = 0.0f, fCos2 = 0.0f, fRads = 0.0f, fDiagRad = 0.0f;
	float dx = 0.0f, dy = 0.0f;
	float fRotSpd = MOUSE_SENS * PLAYER_SPEED * fElapsedTime;

	switch (pMove) {

	case PlayerMovDir::FWD:
		dx += fSin;
		dy += fCos;
		break;

	case PlayerMovDir::BACK:
		dx += -fSin;
		dy += -fCos;
		break;

	case PlayerMovDir::LEFT:
		fAngle = ROT_RADS(fAngle + fRotSpd);
		break;

	case PlayerMovDir::RIGHT:
		fAngle = ROT_RADS(fAngle - fRotSpd);
		break;

	case PlayerMovDir::STRLEFT:
		dx += fCos;
		dy += -fSin;
		break;

	case PlayerMovDir::STRRIGHT:
		dx += -fCos;
		dy += fSin;
		break;

	case PlayerMovDir::FWDDIAGLEFT:
	case PlayerMovDir::BKDIAGLEFT:
		fDiagRad = DEGS_TO_RADS(45);
		fRads = ROT_RADS(fAngle + fDiagRad);
		fSin2 = std::sin(fRads);
		fCos2 = std::cos(fRads);
		dx += fSin2;
		dy += fCos2;
		break;

	case PlayerMovDir::FWDDIAGRIGHT:
	case PlayerMovDir::BKDIAGRIGHT:
		fDiagRad = DEGS_TO_RADS(45);
		fRads = ROT_RADS(fAngle - fDiagRad);
		fSin2 = std::sin(fRads);
		fCos2 = std::cos(fRads);
		dx += fSin2;
		dy += fCos2;
		break;
	}

	if (std::abs(fAngle) > M_RAD_DEGS_360) {
		if (fAngle > 0) fAngle -= M_RAD_DEGS_360;
		else fAngle += M_RAD_DEGS_360;
	}

	// Normalise
	updatePlayerPositionWithWallDetection(dx * PLAYER_SPEED * fElapsedTime, dy * PLAYER_SPEED * fElapsedTime);
}

void GamePlayer::update(float fElapsedTime, PlayerMovDir moveDir) {
	pMove = moveDir;
	movement(fElapsedTime);
}

olc::vf2d GamePlayer::posPlayer() {
	return { coords.x, coords.y };
}

olc::vf2d GamePlayer::dirPlayer() {
	return { vDir.x, vDir.y };
}

olc::vi2d GamePlayer::posMap() {
	return { (int)coords.x, (int)coords.y };
}

void GamePlayer::render(olc::PixelGameEngine* pge) {
	drawVectors(pge);
	pge->FillRectDecal({ coords.x * GAME_GRID_PX_SIZE_X - 5, coords.y * GAME_GRID_PX_SIZE_Y - 5 }, { 10, 10 }, olc::CYAN);
	pge->DrawStringDecal({ 10, 10 }, "Movement: " + moveDirToStr(pMove) 
		+ ", Radians: " + std::to_string(fAngle) 
		+ ", Angle: " + std::to_string(fAngle * M_RAD_DEG)
		+ ", dx: " + std::to_string((int)(coords.x * GAME_GRID_PX_SIZE_X))
		+ ", dy: " + std::to_string((int)(coords.y * GAME_GRID_PX_SIZE_Y))
	);
}

void GamePlayer::drawVectors(olc::PixelGameEngine* pge) {
	if (!gEngine->bDrawRays) {

	}


	if (gEngine->bDrawPlayerVector) {
		olc::Pixel cVector = olc::YELLOW;
		if (gEngine->bDrawRays)
			cVector = olc::BLACK;

		float sx = coords.x * GAME_GRID_PX_SIZE_X;
		float sy = coords.y * GAME_GRID_PX_SIZE_Y;
		float dx = (coords.x * GAME_GRID_PX_SIZE_X + 50 * vDir.x);
		float dy = (coords.y * GAME_GRID_PX_SIZE_Y + 50 * vDir.y);
		olc::vf2d fd = { dx, dy };


		// Draw vector
		pge->DrawLineDecal({ sx, sy }, fd, cVector);

		dx = fd.x - 5 * std::sin(fAngle - DEGS_TO_RADS(45));
		dy = fd.y - 5 * std::cos(fAngle - DEGS_TO_RADS(45));
		pge->DrawLineDecal(fd, { dx, dy }, cVector);

		dx = fd.x - 5 * std::sin(fAngle + DEGS_TO_RADS(45));
		dy = fd.y - 5 * std::cos(fAngle + DEGS_TO_RADS(45));
		pge->DrawLineDecal(fd, { dx, dy }, cVector);
	}
}

std::string GamePlayer::moveDirToStr(PlayerMovDir pMove) {
	switch (pMove) {
	case PlayerMovDir::FWD: return "FORWARD";
	case PlayerMovDir::BACK: return "BACK";
	case PlayerMovDir::LEFT: return "LEFT";
	case PlayerMovDir::RIGHT: return "RIGHT";
	case PlayerMovDir::STRLEFT: return "STRLEFT";
	case PlayerMovDir::STRRIGHT: return "STRRIGHT";
	case PlayerMovDir::FWDDIAGLEFT: return "FWDDIAGLEFT";
	case PlayerMovDir::FWDDIAGRIGHT: return "FWDDIAGRIGHT";
	case PlayerMovDir::BKDIAGLEFT: return "BKDIAGLEFT";
	case PlayerMovDir::BKDIAGRIGHT: return "BKDIAGRIGHT";
	default: return "NONE";
	}
}

void GamePlayer::reset() {
	coords = { 1.5, 5 };
	fAngle = 0.0f;
}

bool GamePlayer::checkWall(olc::vi2d vec) {
	return gEngine->gMap->checkIntercept(vec);
}

void GamePlayer::updatePlayerPositionWithWallDetection(float dx, float dy) {
	auto vec = posPlayer();
	if (!checkWall({ (int)(vec.x + dx), (int)(vec.y) })) coords.x += dx;
	if (!checkWall({ (int)(vec.x), (int)(vec.y + dy) })) coords.y += dy;

	vDir.x = std::sin(fAngle);
	vDir.y = std::cos(fAngle);
	// Normalise from point 0, where point zero is {0,0}
	// 0.25 is right, 0.5 is top, 0.75 is left, 1.0 or 0.0 is straight down
	// positive x from 0.0-0.5, negative x from 0.5-1.0
	// positive y from 0.25-0.75, negative y from 0.0-0.25 & 0.75-1.0
	//if (normRad <= 0) return; // Avoid divide by zero

	//if (fAngle < 0.25) {
	//	vDir.x = normRad / 0.25;
	//	vDir.y = 1 - normRad / 0.25;
	//}
	//else if (fAngle < 0.5) {
	//	vDir.x = 1 - normRad / 0.5;
	//	vDir.y = normRad / 0.5;
	//}
	//else if (fAngle < 0.75) {
	//	vDir.x = 0 - (normRad / 0.75);
	//	vDir.y = 0 - (1 - normRad / 0.75);
	//}
	//else {
	//	vDir.x = 0 - (1 - normRad / 1.0);
	//	vDir.y = 0 - (normRad / 1.0);
	//}
	
}

float GamePlayer::playerAngle() {
	return fAngle;
}