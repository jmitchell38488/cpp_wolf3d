#ifndef GAME_PLAYER
#define GAME_PLAYER
#pragma once

#include <olcPixelGameEngine.h>

enum class PlayerMovDir {
	NONE, FWD, BACK, LEFT, RIGHT, STRLEFT, STRRIGHT, // 1,2,3,4,5,6
	FWDLEFT, FWDRIGHT, BKLEFT, BKRIGHT, // 7, 8, 9, 10
	FWDDIAGLEFT, FWDDIAGRIGHT, // (10+) 16, 17
	BKDIAGLEFT, BKDIAGRIGHT // (20+) 27, 28, 
};

class GameEngine;

class GamePlayer {
public:
	olc::vf2d coords;
	olc::vf2d vDir;
	float fAngle;
  float fFov;
	PlayerMovDir pMove{ 0 };
	bool bRunning;

public:
	GamePlayer();
	GamePlayer(GameEngine* engine);

public:
	void update(float fElapsedTime, PlayerMovDir moveDir);
	olc::vf2d posPlayer();
	olc::vf2d dirPlayer();
	olc::vi2d posMap();
	void render(olc::PixelGameEngine* pge);
	void reset();
	bool checkWall(olc::vi2d vec);
	void updatePlayerPositionWithWallDetection(float dx, float dy);
	float playerAngle();
	std::string pMoveDirToStr();
	void pMoveStrToDir(const std::string pMove);

protected:
	void movement(float fElapsedTime);

private:
	void drawVectors(olc::PixelGameEngine* pge);

private:
	GameEngine* gEngine = nullptr;
};

#endif // GAME_PLAYER