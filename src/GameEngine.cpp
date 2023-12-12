#include "GameEngine.h"
#include "Definitions.h"
#include "GameMap.h"
#include "GamePlayer.h"
#include "Raycaster/Raycaster_Step.h"
#include "ObjectRenderer.h"
#include "GameMap.h"
#include "Settings.h"
#include "Util.h"
#include <iostream>
#include <string.h>
using namespace std;

GameEngine::GameEngine() {
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	fAccumulatedTime = 0.0f;
}

GameEngine::GameEngine(GameSettings* settings) : GameEngine() {
	gSettings = settings;
	setLod(3);
}

float GameEngine::getRunTime() {
	m_tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
	return elapsedTime.count();
}

void GameEngine::handleInput(float fElapsedTime) {
	if (!pge->IsConsoleShowing()) {
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
	}

	if (pge->GetKey(olc::Key::SHIFT).bHeld) {
		if (gPlayer->bRunning) gPlayer->bRunning = false;
	}

	if (pge->GetKey(olc::Key::SHIFT).bReleased) {
		gPlayer->bRunning = true;
	}

	if (pge->GetKey(olc::Key::OEM_3).bPressed) {
		pge->ConsoleShow(olc::Key::OEM_3, true);
		pge->ConsoleCaptureStdOut(true);
	}
}

bool GameEngine::OnConsoleCommand(const std::string& sCommand) {
	if (sCommand == "exit" || sCommand == "quit") {
		exit(0);
	}

	std::vector<std::string> tokens;
	split(sCommand, tokens, ' ');

	std::string key = "";
	if (!tokens.empty()) {
		key = "window.size";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "Window (" << gSettings->Window.Width << ", " << gSettings->Window.Height << ")" << endl;
		}

		key = "bDrawRays";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "bDrawRays=" << bDrawRays << endl;
			if (tokens.size() == 2) {
				if (tokens[1].compare("1") == 0) bDrawRays = true;
				if (tokens[1].compare("0") == 0) bDrawRays = false;
			}
		}

		key = "debug";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "debug=" << gSettings->Game.DebugMode << endl;
			if (tokens.size() == 2) {
				if (tokens[1].compare("1") == 0) gSettings->Game.DebugMode = true;
				if (tokens[1].compare("0") == 0) gSettings->Game.DebugMode = false;
			}
		}

		key = "view.mode";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "view.mode=" << (renderMode == GameRenderMode::TOP ? "2d" : "projected 3d") << endl;
			if (tokens.size() == 2) {
				if (tokens[1].compare("2d") == 0) renderMode = GameRenderMode::TOP;
				if (tokens[1].compare("3d") == 0) renderMode = GameRenderMode::PROJECTED;
			}
		}

		key = "camera.proj.scale";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "camera.proj.scale=" << gSettings->Camera.ProjScale << endl;
			if (tokens.size() == 2) {
				float num = std::stof(tokens[1]);
				gSettings->Camera.ProjScale = num;
			}
		}

		key = "camera.fov";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "camera.fov=" << RADS_TO_DEGS(gSettings->Camera.ProjScale) << endl;
			if (tokens.size() == 2) {
				int num = std::stoi(tokens[1]);
				if (num > 130 || num < 45) cout << "Bounds: 45 - 130" << endl;
				else {
					gSettings->Camera.DeltaAngle = gSettings->Camera.Fov / gSettings->Camera.Rays;
					gSettings->Camera.Fov = DEGS_TO_RADS(num);
					gSettings->Camera.FovHalf = DEGS_TO_RADS(num) / 2;
				}
			}
		}

		key = "camera.rays";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "camera.rays=" << (int)gSettings->Camera.Rays << endl;
			if (tokens.size() == 2) {
				int num = std::stoi(tokens[1]);
				int max = 500;
				if (num > max || num < 50) cout << "Bounds: 50 - 500" << max << endl;
				else {
					gSettings->Camera.Rays = num;
					gSettings->Camera.DeltaAngle = gSettings->Camera.Fov / num;
					gSettings->Camera.Scale = gSettings->Window.Width / num;
				}
			}
		}

		key = "camera.angle";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "camera.angle=" << gSettings->Camera.DeltaAngle << endl;
		}

		key = "camera.view.depth";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "camera.view.depth=" << gSettings->Camera.MaxDepth << endl;
			if (tokens.size() == 2) {
				float num = std::stof(tokens[1]);
				if (num > 20 || num < 1) cout << "Bounds: 1 - 20" << endl;
				else gSettings->Camera.MaxDepth = num;
			}
		}

		key = "player.speed";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "player.speed=" << gSettings->Player.Speed << endl;
			if (tokens.size() == 2) {
				float num = std::stof(tokens[1]);
				if (num > 10 || num < 0) cout << "Bounds: 1 - 10" << endl;
				else {
					gSettings->Player.Speed = num;
					gSettings->Player.WlkSpeed = num / 2;
				}
			}
		}

		key = "lod";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "lod=" << lod << endl;
			if (tokens.size() == 2) {
				float num = std::stof(tokens[1]);
				if (num > 5 || num < 1) cout << "Bounds: 1 - 5" << endl;
				setLod(num);
			}
		}

		key = "position";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			if (tokens.size() == 1)
				cout << "position{" 
					<< (float)(gPlayer->coords.x * gSettings->Grid.SizeX)
					<< "," 
					<< (float)(gPlayer->coords.y * gSettings->Grid.SizeY)
					<< "," 
					<< gPlayer->fAngle 
					<< "}" << endl;
		}

		key = "list";
		tolowercase(key);
		if (tokens[0].compare(key) == 0) {
			std::vector<std::string> cmds{"window.size","bDrawRays","debug","view.mode",
			"camera.proj.scale","camera.fov","camera.view.depth","camera.fov","camera.rays",
			"camera.angle","player.speed","debug","quit"};
			for (auto c : cmds) cout << c << endl;
		}
		


	}

	tokens.clear();

	return true;
}

// See: https://docs.unity3d.com/Manual/ExecutionOrder.html
void GameEngine::doGameUpdate(float fElapsedTime) {
	while (fAccumulatedTime >= fElapsedTime) {
		// gScriptProcessor->processCommands(fElapsedTime);
		// Perform some other updates in here, including animation updates, AI updates, fire animations, etc

		if (!pge->IsConsoleShowing()) {
			gPlayer->update(fElapsedTime, getPlayerMoveDir());
			gRaycasterStep->update(fElapsedTime);
		}

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
	gRaycasterStep = std::make_unique<Raycaster_Step>(this);
	gObjRenderer = std::make_unique<ObjectRenderer>(this);

	pge = engine;

	return true;
}

void GameEngine::render() {
	pge->Clear(olc::BLACK);
	olc::Pixel px = olc::DARK_GREY;

	if (renderMode == GameRenderMode::TOP) {
		for (auto pos : gMap->coords)
			pge->DrawRectDecal({ pos.x * GAME_GRID_PX_SIZE_X, pos.y * GAME_GRID_PX_SIZE_Y }, { GAME_GRID_PX_SIZE_X, GAME_GRID_PX_SIZE_Y }, px);
	}

	gRaycasterStep->render(pge);
	gPlayer->render(pge);

	if (gSettings->Game.DebugMode) {
		pge->DrawStringDecal({ 10, 10 }, "Movement: " + gPlayer->pMoveDirToStr()
			+ ", Radians: " + std::to_string(gPlayer->fAngle)
			+ ", Angle: " + std::to_string(RADS_TO_DEGS(gPlayer->fAngle))
			+ ", dx: " + std::to_string((int)(gPlayer->coords.x * gSettings->Grid.SizeX))
			+ ", dy: " + std::to_string((int)(gPlayer->coords.y * gSettings->Grid.SizeY))
			+ ", lod: " + std::to_string(lod)
			+ ", rays: " + std::to_string((int)gSettings->Camera.Rays)
			+ ", pos: {" + std::to_string((int)(gPlayer->coords.x * gSettings->Grid.SizeX))
				+ "," + std::to_string((int)(gPlayer->coords.y * gSettings->Grid.SizeY))
				+ "," + std::to_string(gPlayer->fAngle)
				+ "}"
			, olc::WHITE, {0.5f, 0.5f}
		);
	}
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

GameEngine::LOD GameEngine::getLod() {
	LOD l = lods[lod-1];
	return GameEngine::LOD{l.rays, l.text, l.ddist, l.llevel}; // copy it instead of passing the object or a reference to it
}

void GameEngine::setLod(uint8_t l) {
	if (l < 1 || l > lods.size()+1)
		return;

	lod = l;

	gSettings->Camera.Rays = lods[lod-1].rays;
	gSettings->Camera.DeltaAngle = gSettings->Camera.Fov / lods[lod-1].rays;
	gSettings->Camera.Scale = gSettings->Window.Width / lods[lod-1].rays;
}