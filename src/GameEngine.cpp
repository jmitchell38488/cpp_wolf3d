#include "GameEngine.h"
#include "Definitions.h"
#include "GameMap.h"
#include "GamePlayer.h"

GameEngine::GameEngine() {
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	fAccumulatedTime = 0.0f;

}

float GameEngine::getRunTime() {
	m_tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
	return elapsedTime.count();
}

void GameEngine::handleInput(float fElapsedTime) {
	if (pge->GetKey(olc::Key::Q).bPressed) {
		exit(0);
	}

	if (pge->GetKey(olc::Key::ESCAPE).bPressed) {
		resetGame();
	}
}

// See: https://docs.unity3d.com/Manual/ExecutionOrder.html
void GameEngine::doGameUpdate(float fElapsedTime) {
	while (fAccumulatedTime >= fElapsedTime) {
		// gScriptProcessor->processCommands(fElapsedTime);
		// Perform some other updates in here, including animation updates, AI updates, fire animations, etc

		gPlayer->update(fElapsedTime, getPlayerMoveDir());

		// Decrement timer
		fAccumulatedTime -= fElapsedTime;
	}
}

bool GameEngine::update(float fElapsedTime) {
	fAccumulatedTime += fElapsedTime;

	if (fAccumulatedTime < 0)
		fAccumulatedTime = 0.0f;

	handleInput(fAccumulatedTime);
	doGameUpdate(GAME_TICK);

	render();

	return true;
}

void GameEngine::resetGame() {
	gPlayer->reset();
}

bool GameEngine::initialise(olc::PixelGameEngine* engine) {
	gMap = std::make_unique<GameMap>(initialise_map());
	gPlayer = std::make_unique<GamePlayer>(this);
	pge = engine;

	return true;
}

void GameEngine::render() {
	pge->Clear(olc::BLACK);
	olc::Pixel px = olc::DARK_GREY;

	for (auto pos : gMap->coords)
		pge->DrawRectDecal({ pos.x * GAME_GRID_PX_SIZE_X, pos.y * GAME_GRID_PX_SIZE_Y }, { GAME_GRID_PX_SIZE_X, GAME_GRID_PX_SIZE_Y }, px);

	gPlayer->render(pge);
}

PlayerMovDir GameEngine::getPlayerMoveDir() {
	uint8_t pMove = 0;

	bool bUp = false;
	bool bDown = false;
	bool bLeft = false;
	bool bRight = false;
	bool bStrLeft = false;
	bool bStrRight = false;

	// W or up arrow
	if (pge->GetKey(olc::Key::W).bPressed || pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bPressed || pge->GetKey(olc::Key::UP).bHeld)
		bUp = true;

	// S or down arrow
	if (pge->GetKey(olc::Key::S).bPressed || pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bPressed || pge->GetKey(olc::Key::DOWN).bHeld)
		bDown = true;

	// left arrow
	if (pge->GetKey(olc::Key::LEFT).bPressed || pge->GetKey(olc::Key::LEFT).bHeld)
		bLeft = true;

	// right arrow
	if (pge->GetKey(olc::Key::RIGHT).bPressed || pge->GetKey(olc::Key::RIGHT).bHeld)
		bRight = true;

	// strafe left
	if (pge->GetKey(olc::Key::A).bPressed || pge->GetKey(olc::Key::A).bHeld)
		bStrLeft = true;

	// strafe right
	if (pge->GetKey(olc::Key::D).bPressed || pge->GetKey(olc::Key::D).bHeld)
		bStrRight = true;

	// Overrides for strafing
	if (bStrRight) bRight = false;
	if (bStrLeft) bLeft = false;

	// negate
	if (bUp && bDown) bUp = bDown = false;
	if (bStrLeft && bStrRight) bStrLeft = bStrRight = false;
	if (bLeft && bRight) bLeft = bRight = false;

	if (bUp) pMove += 1;
	if (bDown) pMove += 2;
	if (bLeft) pMove += 3;
	if (bRight) pMove += 4;
	if (bStrLeft) pMove += 5;
	if (bStrRight) pMove += 6;

	if (bStrLeft && bUp || bStrRight && bUp) pMove += 10;
	if (bStrLeft && bDown || bStrRight && bDown) pMove += 20;

	switch (pMove) {
		case 0: return PlayerMovDir::NONE;
		case 1: return PlayerMovDir::FWD;
		case 2: return PlayerMovDir::BACK;
		case 3: return PlayerMovDir::LEFT;
		case 4: return PlayerMovDir::RIGHT;
		case 5: return PlayerMovDir::STRLEFT;
		case 6: return PlayerMovDir::STRRIGHT;
		case 16: return PlayerMovDir::FWDDIAGLEFT;
		case 17: return PlayerMovDir::FWDDIAGRIGHT;
		case 27: return PlayerMovDir::BKDIAGLEFT;
		case 28: return PlayerMovDir::BKDIAGRIGHT;
	}

	return PlayerMovDir::NONE;
}