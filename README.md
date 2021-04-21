![Odeum Logo](/media/odeumlogo.png)

This is the repository for our Game Programming capstone project at Humber College, developed by [Aidan Zizys](https://aidanzizysgamedev.com), [Benjamin Sayliss](https://benjaminsaylissgamedev.com), [Robert Ward](https://robertwardgamedev.com), [Ryan Thorne](https://www.ryanthornegamedev.com), and [Diego Gregori](https://www.diegogregorigamedev.com). 

For our capstone, we built a DirectX 12 based game engine entirely in C/C++, which we started writing in late August, 2020. In order to show off these features, we built a Mario-party style game entirely using our engine, which we started working on in January, 2021.

Feel free to check out our [wiki](https://github.com/azgame/Odeum/wiki), or navigate directly to our [engine code](Odeum/OdeumEngine/OdeumEngine/Engine) or our [game code](Odeum/OdeumEngine/OdeumEngine/Game). You can also find more media, such as our technical design document or our game engine architecture in our [media folder](media).
<br></br>
<br></br>
<p align="center">Check out a trailer for our engine and our game below!</p>
<p align="center"><a href="https://www.youtube.com/watch?v=PAHP0r7vL6A"><img src="https://img.youtube.com/vi/PAHP0r7vL6A/0.jpg"></a></p>

## OdeumEngine Features

Our engine features an advanced renderer, fully supported object and material loading, custom shaders in HLSL, compute rendering, real-time physics, animations, scene creation and management, an entity-component system, multithreading, custom navigation mesh creation, audio, input, dynamic window sizing and support for any aspect ratio, event handling, UI via ImGui, File I/O, and custom data structures and design patterns. 

## Game Highlights

Players move around a small board game. Each space has a different function (play a minigame, gain gold, lose gold etc.). Players compete in minigames, with the winner gaining gold. The player with the most gold at the end of the last round will win the game. A mix of both luck and skill is required!

The game features an opening menu that allows the player to play the game normally, or pick from a list of hand-crafted minigames. 

## Using our engine

In order to use our engine to make a game, create an instance of OdeumEngine, and initialize it with an instance of a GameInterface in your entry point.

```C++
  OdeumEngine* app;
  app = new OdeumEngine();
  app->SetGameInterface(new Game1);
  ASSERT(app != nullptr, "App is null!");

  if (app->Initialize())
	  app->Run();

  SAFE_DELETE(app);
```

From here, simply use the concrete implementation of the GameInterface to list and determine which concretely implemented scenes to load, given a new scene index. Scenes (read: levels) represent the sections of gameplay that the player will experience.

## Expanded Credits

Aidan Zizys - Engine, Graphics, Systems programmer
  * [Website](https://aidanzizysgamedev.com) | [Github](https://github.com/azgame) | [LinkedIn](https://www.linkedin.com/in/aidan-zizys/)

Benjamin Sayliss - Engine, Physics programmer
  * [Website](https://benjaminsaylissgamedev.com) | [Github](https://github.com/BenSayliss) | [LinkedIn](https://www.linkedin.com/in/benjamin-sayliss-407311177/)

Robert Ward - Gameplay programmer
  * [Website](https://robertwardgamedev.com) | [Github](https://github.com/docdodo) | [LinkedIn](https://www.linkedin.com/in/robert-ward-710234175/)

Ryan Thorne - Generalist programmer
  * [Website](https://www.ryanthornegamedev.com) | [Github](https://github.com/mongoose9304) | [LinkedIn](https://www.linkedin.com/in/ryan-thorne-250366175/)

Diego Gregori - Generalist programmer
  * [Website](https://www.diegogregorigamedev.com) | [Github](https://github.com/dgmode21) | [LinkedIn](https://www.linkedin.com/in/diego-gregori/)

## Copyrights

We've included copyright notices for all of the assets (models, textures, sound, etc.) we used in our game. They can be found in the copyright pdf in this folder.

We used a few C++ libraries to help build our engine, including FMOD, ImGui, Nlohmann's JSON library, AssImp, and we used a number of files from Microsoft's Mini Engine sample set for developing our renderer. These files include their original licenses.
