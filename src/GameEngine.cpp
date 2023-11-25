#include "GameEngine.h"
#include "Definitions.h"
#include "GameMap.h"
#include "GamePlayer.h"

GameEngine::GameEngine() {
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
}

float GameEngine::getRunTime() {
	m_tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
	return elapsedTime.count();
}

void GameEngine::handleInput(float fElapsedTime, olc::PixelGameEngine* pge) {
	if (pge->GetKey(olc::Key::Q).bPressed) {
		exit(0);
	}
}

// See: https://docs.unity3d.com/Manual/ExecutionOrder.html
void GameEngine::doGameUpdate(float fElapsedTime, olc::PixelGameEngine* pge) {
	while (fAccumulatedTime >= fElapsedTime) {
		// gScriptProcessor->processCommands(fElapsedTime);
		// Perform some other updates in here, including animation updates, AI updates, fire animations, etc
		fAccumulatedTime -= fElapsedTime;
	}
}

bool GameEngine::update(float fElapsedTime, olc::PixelGameEngine* pge) {
	fAccumulatedTime += fElapsedTime;

	if (fAccumulatedTime < 0)
		fAccumulatedTime = 0.0f;

	handleInput(fAccumulatedTime, pge);
	doGameUpdate(GAME_TICK, pge);

	render(pge);

	return true;
}

void GameEngine::resetGame() {

}

bool GameEngine::initialise() {
	gMap = std::make_unique<GameMap>(initialise_map());
	gPlayer = new GamePlayer();

	return true;
}

void GameEngine::render(olc::PixelGameEngine* pge) {
	pge->Clear(olc::BLACK);
	olc::Pixel px = olc::DARK_GREY;

	for (auto pos : gMap->coords)
		pge->DrawRectDecal({ pos.x * GAME_GRID_PX_SIZE_X, pos.y * GAME_GRID_PX_SIZE_Y }, { GAME_GRID_PX_SIZE_X, GAME_GRID_PX_SIZE_Y }, px);

	pge->FillRectDecal({ gPlayer->posPlayer().x * GAME_GRID_PX_SIZE_X, gPlayer->posPlayer().y * GAME_GRID_PX_SIZE_Y }, { 10, 10 }, olc::CYAN);
}