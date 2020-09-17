#include "pch.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Engine/Core/OdeumEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	OdeumEngine* app;
	app = new OdeumEngine();
	ASSERT(app != nullptr, "App is null!");

	if (app->Initialize())
		app->Run();

	app->Uninitialize();
	SAFE_DELETE(app);

	// _CrtDumpMemoryLeaks();

	return 0;
}