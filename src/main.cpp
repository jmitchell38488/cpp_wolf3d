#include "GameEngine.h"
#include "olcLauncher.h"
#include "Settings.h"

class GameEngine;

int main()
{
	auto gSettings = std::make_unique<GameSettings>();
	auto gEngine = std::make_unique<GameEngine>(gSettings.get());
	auto launcher = std::make_unique<olcLauncher>(gEngine.get(), gSettings.get());
	launcher->run();
}