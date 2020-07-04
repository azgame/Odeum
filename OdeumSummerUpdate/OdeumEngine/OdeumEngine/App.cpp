#include "pch.h"

#include <crtdbg.h>

#include "Engine/Core/OdeumEngine.h"

// WINAPI entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	OdeumEngine* app;
	app = new OdeumEngine();
	assert(app);

	if (app->Initialize())
		app->Run();

	app->Uninitialize();
	SAFE_DELETE(app);

	_CrtDumpMemoryLeaks();

	return 0;
}