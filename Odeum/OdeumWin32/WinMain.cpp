#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	App* app;

	app = new App();
	if (!app) return 0;

	if (app->Initialize())
		app->Run();

	app->Uninitialize();
	delete app;
	app = 0;

	return 0;
}