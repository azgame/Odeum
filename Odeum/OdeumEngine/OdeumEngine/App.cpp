#include "pch.h"
#include "Game/Game1.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// #include "Engine/Core/OdeumEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	FILE* fp;

	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);

	OdeumEngine* app;
	app = new OdeumEngine();
	app->SetGameInterface(new Game1);
	ASSERT(app != nullptr, "App is null!");

	if (app->Initialize())
		app->Run();

	app->Uninitialize();
	SAFE_DELETE(app);
	// _CrtDumpMemoryLeaks();

	return 0;
}