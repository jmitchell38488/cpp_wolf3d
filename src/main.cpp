#include "GameEngine.h"
#include "olcLauncher.h"
#include "Settings.h"

class GameEngine;

int main()
{
	auto gEngine = std::make_unique<GameEngine>();
	auto launcher = std::make_unique<olcLauncher>(gEngine.get());
	launcher->run();
}