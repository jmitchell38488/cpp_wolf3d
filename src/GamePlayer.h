#ifndef GAME_PLAYER
#define GAME_PLAYER
#pragma once

#include <olcPixelGameEngine.h>

enum class PlayerMovDir {
	NONE, FWD, BACK, LEFT, RIGHT, STRLEFT, STRRIGHT, // 1,2,3,4,5,6
	FWDDIAGLEFT, FWDDIAGRIGHT, // (10+) 16, 17
	BKDIAGLEFT, BKDIAGRIGHT // (20+) 27, 28, 
};

class GamePlayer {
protected:
	olc::vf2d coords;
	float fAngle;
	PlayerMovDir pMove;

public:
	GamePlayer();

public:
	void update(float fElapsedTime, PlayerMovDir moveDir);
	olc::vf2d posPlayer();
	olc::vi2d posMap();
	void render(olc::PixelGameEngine* pge);
	void reset();

protected:
	void movement(float fElapsedTime);
	std::string moveDirToStr(PlayerMovDir pMove);
};

#endif // GAME_PLAYER