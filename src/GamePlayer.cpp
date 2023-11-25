#include "GamePlayer.h"
#include "Definitions.h"
#include <olcPixelGameEngine.h>

GamePlayer::GamePlayer() {
	coords = { 1.5, 5 };
	fAngle = 0.0f;
	fSpeed = 0.0f;
	fRotSpeed = 0.0f;
}

GamePlayer::~GamePlayer() {

}

void GamePlayer::movement(float fElapsedTime ) {
	float fSin = std::sin(fAngle);
	float fCos = std::cos(fAngle);
	float dx, dy = 0.0f;
	fSpeed = PLAYER_SPEED * fElapsedTime;
	float fSpdSin = fSpeed * fSin;
	float fSpdCos = fSpeed * fCos;


}

void GamePlayer::update(float fElapsedTime) {

}

olc::vf2d GamePlayer::posPlayer() {
	return { coords.x, coords.y };
}

olc::vi2d GamePlayer::posMap() {
	return { (int)coords.x, (int)coords.y };
}