#include <olcPixelGameEngine.h>
#include "olcLauncher.h"
#include "GameEngine.h"
#include "Settings.h"

olcLauncher::olcLauncher() {
	sAppName = "Wolf3D";
}

olcLauncher::olcLauncher(GameEngine* engine, GameSettings* settings) : olcLauncher() {
	gEngine = engine;
	gSettings = settings;
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
	if (Construct(
		gSettings->Window.Width,
		gSettings->Window.Height,
		gSettings->Window.Pixel,
		gSettings->Window.Pixel,
		gSettings->Window.FullScreen,
		gSettings->Window.VSync,
		gSettings->Window.PxCoh))
	{
		Start();
	}
}

bool olcLauncher::OnConsoleCommand(const std::string& sCommand) {
	return gEngine->OnConsoleCommand(sCommand);
}