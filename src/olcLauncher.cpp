#include <olcPixelGameEngine.h>
#include "olcLauncher.h"
#include "GameEngine.h"
#include "Settings.h"

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
	GameSettings gSettings = GameSettings::Get();
	if (Construct(
		gSettings.Window.Width,
		gSettings.Window.Height,
		gSettings.Window.Pixel,
		gSettings.Window.Pixel,
		gSettings.Window.FullScreen,
		gSettings.Window.VSync,
		gSettings.Window.PxCoh))
	{
		Start();
	}
}

bool olcLauncher::OnConsoleCommand(const std::string& sCommand) {
	return gEngine->OnConsoleCommand(sCommand);
}