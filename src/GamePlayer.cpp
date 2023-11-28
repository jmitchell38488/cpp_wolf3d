#include "GamePlayer.h"
#include "Definitions.h"
#include <olcPixelGameEngine.h>

#define DEGS_TO_RADS(x) (x * M_DEG_RAD)
#define RADS_TO_DEGS(x) (x * M_RAD_DEG)
#define ROT_RADS(x) (x < 0 ? M_RAD_DEGS_360 - x : x)
#define DEG_NORM 1/360;
#define RAD_NORM DEGS_TO_RADS(1)

GamePlayer::GamePlayer() {
	coords = { 1.5, 5 };
	fAngle = 0.0f;
}

void GamePlayer::movement(float fElapsedTime) {
	// No movement to calculate
	if (pMove == PlayerMovDir::NONE) {
		return;
	}

	float fSin = std::sin(fAngle);
	float fCos = std::cos(fAngle);
	float fTan = std::tan(fAngle);
	float fSin2 = 0.0f, fCos2 = 0.0f, fTan2 = 0.0f, fDegs = 0.0f, fDegs2 = 0.0f, fDegs3 = 0.0f, fRads = 0.0f, fDiagRad = 0.0f, fRads2 = 0.0f;
	float dx = 0.0f, dy = 0.0f;
	float fRotSpd = MOUSE_SENS * PLAYER_SPEED * fElapsedTime;
	float fPiAngle = (M_PI * 2 / 360);
	float fRadOff = DEGS_TO_RADS(90);

	// Adjusting diagonal movement to 1 / sqrt(2)
	// dx += fSin * M_SQRT1_2;
	// dy += fCos * M_SQRT1_2;

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
	coords.x += dx * PLAYER_SPEED * fElapsedTime;
	coords.y += dy * PLAYER_SPEED * fElapsedTime;

	//fAngle = std::fmod(fAngle, 2 * M_PI);
}

void GamePlayer::update(float fElapsedTime, PlayerMovDir moveDir) {
	pMove = moveDir;
	movement(fElapsedTime);
}

olc::vf2d GamePlayer::posPlayer() {
	return { coords.x, coords.y };
}

olc::vi2d GamePlayer::posMap() {
	return { (int)coords.x, (int)coords.y };
}

void GamePlayer::render(olc::PixelGameEngine* pge) {
	float fMaxRads = M_RAD_DEGS_360 / 2;
	float fAngleX = fAngle > fMaxRads ? (0 - fAngle) + fMaxRads : fAngle; // (-180 degs 180 degs)
	float fVectAngle = fAngleX / fMaxRads;
	float fY = std::cos(fVectAngle);
	float fX = std::sin(fVectAngle);
	
	float dx = (coords.x * GAME_GRID_PX_SIZE_X + 50 * std::sin(fAngle));
	float dy = (coords.y * GAME_GRID_PX_SIZE_Y + 50 * std::cos(fAngle));
	olc::vf2d fd = { dx, dy };q
	// Draw vector
	pge->DrawLineDecal({ coords.x * GAME_GRID_PX_SIZE_X, coords.y * GAME_GRID_PX_SIZE_Y }, fd, olc::YELLOW);

	dx = fd.x - 5 * std::sin(fAngle - DEGS_TO_RADS(45));
	dy = fd.y - 5 * std::cos(fAngle - DEGS_TO_RADS(45));
	pge->DrawLineDecal(fd, { dx, dy }, olc::YELLOW);

	dx = fd.x - 5 * std::sin(fAngle + DEGS_TO_RADS(45));
	dy = fd.y - 5 * std::cos(fAngle + DEGS_TO_RADS(45));
	pge->DrawLineDecal(fd, { dx, dy }, olc::YELLOW);


	pge->FillRectDecal({ coords.x * GAME_GRID_PX_SIZE_X - 5, coords.y * GAME_GRID_PX_SIZE_Y - 5 }, { 10, 10 }, olc::CYAN);
	pge->DrawStringDecal({ 10, 10 }, "Movement: " + moveDirToStr(pMove) + ", Radians: " + std::to_string(fAngle) + ", Angle: " + std::to_string(fAngle * M_RAD_DEG));
}

std::string  GamePlayer::moveDirToStr(PlayerMovDir pMove) {
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