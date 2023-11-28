#include <olcPixelGameEngine.h>
#include "olcLauncher.h"
#include "GameEngine.h"
#include "Definitions.h"

olcLauncher::olcLauncher() {
	sAppName = "Wolf3D";
}

olcLauncher::olcLauncher(GameEngine* engine) : olcLauncher() {
	gEngine = engine;
}

olcLauncher::~olcLauncher() {
	// Do nothing yet
}

bool olcLauncher::OnUserCreate() {
	return gEngine->initialise(this);
}

bool olcLauncher::OnUserUpdate(float fElapsedTime) {
	return gEngine->update(fElapsedTime);
}

void olcLauncher::run() {
	if (Construct(GAME_WIDTH, GAME_HEIGHT, GAME_PIXEL, GAME_PIXEL, FS_MODE, VSYNC_MODE, PX_COHESION))
	{
		Start();
	}
}