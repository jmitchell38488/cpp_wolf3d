#ifndef GAME_PLAYER
#define GAME_PLAYER
#pragma once

#include <olcPixelGameEngine.h>

enum class PlayerMovDir {
	UP, DOWN, LEFT, RIGHT, STRLEFT, STRRIGHT, // 1,2,3,4,5,6
	FWDDIAGLEFT, FWDDIAGRIGHT, // (10+) 16, 17
	BKDIAGLEFT, BKDIAGRIGHT // (20+) 27, 28, 
};

class GamePlayer {
protected:
	olc::vf2d coords;
	float fAngle;
	float fSpeed;
	float fRotSpeed;

public:
	GamePlayer();
	~GamePlayer();

public:
	void movement(float fElapsedTime);
	void update(float fElapsedTime);
	olc::vf2d posPlayer();
	olc::vi2d posMap();

};

#endif // GAME_PLAYER