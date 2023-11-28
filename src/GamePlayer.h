#ifndef GAME_PLAYER
#define GAME_PLAYER
#pragma once

#include <olcPixelGameEngine.h>

enum class PlayerMovDir {
	NONE, FWD, BACK, LEFT, RIGHT, STRLEFT, STRRIGHT, // 1,2,3,4,5,6
	FWDDIAGLEFT, FWDDIAGRIGHT, // (10+) 16, 17
	BKDIAGLEFT, BKDIAGRIGHT // (20+) 27, 28, 
};

class GameEngine;

class GamePlayer {
protected:
	olc::vf2d coords;
	float fAngle = 0.0f;
	PlayerMovDir pMove{ 0 };

public:
	GamePlayer();
	GamePlayer(GameEngine* engine);

public:
	void update(float fElapsedTime, PlayerMovDir moveDir);
	olc::vf2d posPlayer();
	olc::vi2d posMap();
	void render(olc::PixelGameEngine* pge);
	void reset();
	bool checkWall(olc::vi2d vec);
	void updatePlayerPositionWithWallDetection(float dx, float dy);

protected:
	void movement(float fElapsedTime);
	std::string moveDirToStr(PlayerMovDir pMove);

private:
	GameEngine* gEngine = nullptr;
};

#endif // GAME_PLAYER