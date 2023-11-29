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

	if (pge->GetKey(olc::Key::R).bPressed) {
		bDrawRays = !bDrawRays;
	}

	if (pge->GetKey(olc::Key::V).bPressed) {
		bDrawPlayerVector = !bDrawPlayerVector;
	}

	if (pge->GetKey(olc::Key::K9).bPressed) {
		renderMode = GameRenderMode::TOP;
	}

	if (pge->GetKey(olc::Key::K0).bPressed) {
		renderMode = GameRenderMode::PROJECTED;
	}
}

// See: https://docs.unity3d.com/Manual/ExecutionOrder.html
void GameEngine::doGameUpdate(float fElapsedTime) {
	while (fAccumulatedTime >= fElapsedTime) {
		// gScriptProcessor->processCommands(fElapsedTime);
		// Perform some other updates in here, including animation updates, AI updates, fire animations, etc

		gPlayer->update(fElapsedTime, getPlayerMoveDir());
		gRaycaster->update(fElapsedTime);

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
	gRaycaster = std::make_unique<Raycaster>(this);

	pge = engine;

	return true;
}

void GameEngine::render() {
	pge->Clear(olc::BLACK);
	olc::Pixel px = olc::DARK_GREY;

	for (auto pos : gMap->coords)
		pge->DrawRectDecal({ pos.x * GAME_GRID_PX_SIZE_X, pos.y * GAME_GRID_PX_SIZE_Y }, { GAME_GRID_PX_SIZE_X, GAME_GRID_PX_SIZE_Y }, px);

	gRaycaster->render(pge);
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

	if (bUp) pMove += 10;
	if (bDown) pMove += 20;

	if (bLeft) pMove += 1;
	if (bRight) pMove += 2;
	if (bStrLeft) pMove += 3;
	if (bStrRight) pMove += 4;

	switch (pMove) {
		case 0: return PlayerMovDir::NONE;
		case 10: return PlayerMovDir::FWD;
		case 20: return PlayerMovDir::BACK;
		case 1: return PlayerMovDir::LEFT;
		case 2: return PlayerMovDir::RIGHT;
		case 3: return PlayerMovDir::STRLEFT;
		case 4: return PlayerMovDir::STRRIGHT;
		// Combo keys forward
		case 11: return PlayerMovDir::FWDLEFT;
		case 12: return PlayerMovDir::FWDRIGHT;
		case 13: return PlayerMovDir::FWDDIAGLEFT;
		case 14: return PlayerMovDir::FWDDIAGRIGHT;
			// Combo keys backward
		case 21: return PlayerMovDir::BKLEFT;
		case 22: return PlayerMovDir::BKRIGHT;
		case 23: return PlayerMovDir::BKDIAGLEFT;
		case 24: return PlayerMovDir::BKDIAGRIGHT;
	}

	return PlayerMovDir::NONE;
}