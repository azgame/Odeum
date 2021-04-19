# Odeum

[Engine Code](Odeum/OdeumEngine/OdeumEngine/Engine)

Initializing

Create an instance of OdeumEngine, and initialize it with an instance of a GameInterface
'''
OdeumEngine* app;
	app = new OdeumEngine();
	app->SetGameInterface(new Game1);
	ASSERT(app != nullptr, "App is null!");

	if (app->Initialize())
		app->Run();

	SAFE_DELETE(app);
'''

[Game Code](Odeum/OdeumEngine/OdeumEngine/Game)
