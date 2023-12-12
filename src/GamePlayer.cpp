#include <olcPixelGameEngine.h>

#include "GamePlayer.h"
#include "Definitions.h"
#include "GameEngine.h"

GamePlayer::GamePlayer() {
	coords = { 1.5, 5 };
	fAngle = M_PI * 0.5f;
  fFov = M_PI * 0.25f;
	vDir = { std::sin(fAngle), std::cos(fAngle) };
	bRunning = true;
}

GamePlayer::GamePlayer(GameEngine * engine) : gEngine(engine) {
	coords = { 1.5, 5 };
	fAngle = M_PI * 0.5f;
  fFov = M_PI * 0.25f;
	vDir = { std::sin(fAngle), std::cos(fAngle) };
	bRunning = true;
}

void GamePlayer::reset() {
	coords = { 1.5, 5 };
	fAngle = M_PI * 0.5f;
  fFov = M_PI * 0.25f;
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
	float fRotSpd = gEngine->gSettings->Player.Sens * gEngine->gSettings->Player.Speed * fElapsedTime;
	float mvSpeed = bRunning ? gEngine->gSettings->Player.Speed : gEngine->gSettings->Player.WlkSpeed;

	bool bFwd = false;
	bool bBack = false;

	switch (pMove) {
		case PlayerMovDir::FWD:
			bFwd = true;
			break;

		case PlayerMovDir::BACK:
			bBack = true;
			break;

		case PlayerMovDir::LEFT:
		case PlayerMovDir::FWDLEFT:
		case PlayerMovDir::BKLEFT:
			fAngle = ROT_RADS(fAngle + fRotSpd);
			if (pMove == PlayerMovDir::FWDLEFT) bFwd = true;
			if (pMove == PlayerMovDir::BKLEFT) bBack = true;
			break;

		case PlayerMovDir::RIGHT:
		case PlayerMovDir::FWDRIGHT:
		case PlayerMovDir::BKRIGHT:
			fAngle = ROT_RADS(fAngle - fRotSpd);
			if (pMove == PlayerMovDir::FWDRIGHT) bFwd = true;
			if (pMove == PlayerMovDir::BKRIGHT) bBack = true;
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

		default:
			break;
	}

	if (bFwd) {
		dx += fSin;
		dy += fCos;
	}

	if (bBack) {
		dx += -fSin;
		dy += -fCos;
	}

	if (std::abs(fAngle) > M_RAD_DEGS_360) {
		if (fAngle > 0) fAngle -= M_RAD_DEGS_360;
		else fAngle += M_RAD_DEGS_360;
	}

	// Normalise

	updatePlayerPositionWithWallDetection(dx * mvSpeed * fElapsedTime, dy * mvSpeed * fElapsedTime);
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

	if (gEngine->renderMode == GameRenderMode::TOP) {
		pge->FillRectDecal({ coords.x * (float)gEngine->gSettings->Grid.SizeX - 5, coords.y * (float)gEngine->gSettings->Grid.SizeY - 5 }, { 10, 10 }, olc::CYAN);
	}
}

void GamePlayer::drawVectors(olc::PixelGameEngine* pge) {
	if (gEngine->renderMode == GameRenderMode::PROJECTED) {
		return;
	}

	if (gEngine->bDrawPlayerVector) {
		olc::Pixel cVector = olc::YELLOW;
		if (gEngine->bDrawRays)
			cVector = olc::BLACK;

		float sx = coords.x * (float)gEngine->gSettings->Grid.SizeX;
		float sy = coords.y * (float)gEngine->gSettings->Grid.SizeY;
		float dx = (coords.x * (float)gEngine->gSettings->Grid.SizeX + 50 * vDir.x);
		float dy = (coords.y * (float)gEngine->gSettings->Grid.SizeY + 50 * vDir.y);
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

bool GamePlayer::checkWall(olc::vi2d vec) {
	return gEngine->gMap->checkIntercept(vec);
}

void GamePlayer::updatePlayerPositionWithWallDetection(float dx, float dy) {
	auto vec = posPlayer();
	if (!checkWall({ (int)(vec.x + dx), (int)(vec.y) })) coords.x += dx;
	if (!checkWall({ (int)(vec.x), (int)(vec.y + dy) })) coords.y += dy;

	vDir.x = std::sin(fAngle);
	vDir.y = std::cos(fAngle);
}

float GamePlayer::playerAngle() {
	return fAngle;
}

std::string GamePlayer::pMoveDirToStr() {
	switch (pMove) {
	case PlayerMovDir::FWD: return "FORWARD";
	case PlayerMovDir::BACK: return "BACK";
	case PlayerMovDir::LEFT: return "LEFT";
	case PlayerMovDir::RIGHT: return "RIGHT";
	case PlayerMovDir::STRLEFT: return "STRLEFT";
	case PlayerMovDir::STRRIGHT: return "STRRIGHT";
	case PlayerMovDir::FWDLEFT: return "FWDLEFT";
	case PlayerMovDir::FWDRIGHT: return "FWDRIGHT";
	case PlayerMovDir::BKLEFT: return "BKLEFT";
	case PlayerMovDir::BKRIGHT: return "BKRIGHT";
	case PlayerMovDir::FWDDIAGLEFT: return "FWDDIAGLEFT";
	case PlayerMovDir::FWDDIAGRIGHT: return "FWDDIAGRIGHT";
	case PlayerMovDir::BKDIAGLEFT: return "BKDIAGLEFT";
	case PlayerMovDir::BKDIAGRIGHT: return "BKDIAGRIGHT";
	default: return "NONE";
	}
}

void GamePlayer::pMoveStrToDir(const std::string move) {
	if (move.compare("NONE") == 0) pMove = PlayerMovDir::NONE;
	if (move.compare("FWD") == 0) pMove = PlayerMovDir::FWD;
	if (move.compare("BACK") == 0) pMove = PlayerMovDir::BACK;
	if (move.compare("LEFT") == 0) pMove = PlayerMovDir::LEFT;
	if (move.compare("RIGHT") == 0) pMove = PlayerMovDir::RIGHT;
	if (move.compare("STRLEFT") == 0) pMove = PlayerMovDir::STRLEFT;
	if (move.compare("STRRIGHT") == 0) pMove = PlayerMovDir::STRRIGHT;
	if (move.compare("FWDLEFT") == 0) pMove = PlayerMovDir::FWDLEFT;
	if (move.compare("FWDRIGHT") == 0) pMove = PlayerMovDir::FWDRIGHT;
	if (move.compare("BKLEFT") == 0) pMove = PlayerMovDir::BKLEFT;
	if (move.compare("BKRIGHT") == 0) pMove = PlayerMovDir::BKRIGHT;
	if (move.compare("BKDIAGLEFT") == 0) pMove = PlayerMovDir::BKDIAGLEFT;
	if (move.compare("BKDIAGRIGHT") == 0) pMove = PlayerMovDir::BKDIAGRIGHT;
}