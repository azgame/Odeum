#include "MapScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
//#include "../Components/PlayerEffectsComponent.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../Game/Components/Player.h"

#include "../Components/PlayerMapComponent.h"

#include "../../Engine/DataStructures/SimpleGraph.h"



MAPScene::MAPScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 25.0f, -19.0f));
	OdeumEngine::Get().GetCamera().SetLookDirection(Vector3(0.0, 0.0, 17.0));

	// player 1 and 2
	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.6f, 1.0f, 0.6f, 0.0f));
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->AddComponent<PlayerMapComponent>();
	//playerObjects.back()->AddComponent<PlayerEffectsComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.5f, 0.0f));
	//playerObjects.back()->AddComponent<Player>(); 
	//playerObjects.back()->GetComponent<Player>()->SetUp(Key::W);
	//playerObjects.back()->GetComponent<Player>()->SetDown(Key::A);
	//playerObjects.back()->GetComponent<Player>()->SetLeft(Key::S);
	//playerObjects.back()->GetComponent<Player>()->SetRight(Key::D);
	playerObjects.back()->SetTag("Player1");

	players.push(playerObjects.back()->GetComponent<PlayerMapComponent>());

	//playerObjects.push_back(new GameObject());
	//playerObjects.back()->AddComponent<Rigidbody>();
	//playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.6f, 1.0f, 0.6f, 0.0f));
	//playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
	//playerObjects.back()->AddComponent<RenderComponent>();
	//playerObjects.back()->AddComponent<Player>();
	////playerObjects.back()->AddComponent<PlayerEffectsComponent>();
	//playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 1.0f));
	//playerObjects.back()->SetTag("Player2");
	//playerObjects.back()->GetComponent<Player>()->SetUp(Key::I);
	//playerObjects.back()->GetComponent<Player>()->SetDown(Key::K);
	//playerObjects.back()->GetComponent<Player>()->SetLeft(Key::J);
	//playerObjects.back()->GetComponent<Player>()->SetRight(Key::L);



	//were gonna tag all your nodes aka game objects via tag to work with our graph.
	/// this is the main starting step white


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 1.0f, 1.0f));
	gameObjects.back()->SetTag("Start");

	map.push(gameObjects.back());


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//left side brown steps
	//connected to start to start white spot


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-3.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(0, 1, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(1, 2, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-9.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(2, 3, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-12.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(3, 4, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-15.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(4, 5, 0);

	// corner

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 4.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(5, 6, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 8.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(6, 7, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 12.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(7, 8, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 16.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(8, 9, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 20.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(9, 10, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 24.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(10, 11, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 28.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(11, 12, 0);

	// top left corner

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.4f, 32.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(12, 13, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 36.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(13, 14, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-15.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(14, 15, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-12.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(15, 16, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-9.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(16, 17, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(17, 18, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-3.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(18, 19, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(19, 20, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(20, 21, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(21, 22, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(9.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(22, 23, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(23, 24, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(24, 25, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, -0.4f, 40.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(25, 26, 0);

	// corner

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(21.0f, -0.4f, 36.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(26, 27, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 32.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(27, 28, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 28.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(28, 29, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 24.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(29, 30, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 20.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(30, 31, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 16.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(31, 32, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 12.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(32, 33, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.4f, 8.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(33, 34, 0);

	// corner

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(21.0f, -0.4f, 4.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(34, 35, 0);

	// corner

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(35, 36, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(36, 37, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachDirectedEdge(37, 38, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(9.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Minigame");

	map.push(gameObjects.back());
	map.attachDirectedEdge(38, 39, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Loss");

	map.push(gameObjects.back());
	map.attachDirectedEdge(39, 40, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	gameObjects.back()->SetTag("Gain");

	map.push(gameObjects.back());
	map.attachDirectedEdge(40, 41, 0);

	//art stuff
	BuildEnviroment();

}

MAPScene::~MAPScene() {
};

void MAPScene::BuildEnviroment()
{
	SetPath(15, true, Vector4(-21.0f, -0.5f, 3.5f, 2.0f));
	SetPath(17, false, Vector4(-18.5f, -0.5f, 1.0f, 2.0f));
	SetPath(15, true, Vector4(24.0f, -0.5f, 3.5f, 2.0f));
	SetPath(17, false, Vector4(-18.5f, -0.5f, 41.0f, 2.0f));

	pathObjects.push_back(new GameObject());
	pathObjects.back()->AddComponent<Rigidbody>();
	pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
	pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.5f, 1.0f, 2.0f));
	pathObjects.back()->AddComponent<RenderComponent>();
	pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathBendBank.fbx");
	pathObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 180.0f);

	pathObjects.push_back(new GameObject());
	pathObjects.back()->AddComponent<Rigidbody>();
	pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
	pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.5f, 1.0f, 2.0f));
	pathObjects.back()->AddComponent<RenderComponent>();
	pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathBendBank.fbx");
	pathObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 90.0f);

	pathObjects.push_back(new GameObject());
	pathObjects.back()->AddComponent<Rigidbody>();
	pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
	pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-21.0f, -0.5f, 41.0f, 2.0f));
	pathObjects.back()->AddComponent<RenderComponent>();
	pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathBendBank.fbx");
	pathObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 270.0f);

	pathObjects.push_back(new GameObject());
	pathObjects.back()->AddComponent<Rigidbody>();
	pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
	pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(24.0f, -0.5f, 41.0f, 2.0f));
	pathObjects.back()->AddComponent<RenderComponent>();
	pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathBendBank.fbx");
	pathObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.0f);

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(4.25, 4.0, 3.8, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(1.5f, -0.5f, 21.0f, 2.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_grass.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -0.5f, 10.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15.0f, -0.5f, 23.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(8.0f, -0.5f, 22.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/stump_oldTall.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(3, 3, 3, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, 1.5f, 17.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/grass_large.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(3, 3, 3, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, 1.5f, 21.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/grass_large.obj");
	

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(3, 3, 3, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(10.0f, 1.5f, 22.3f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/grass_large.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2,2, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 4.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/rock_tallB.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, 0.0f, 3.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/rock_tallB.obj");
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 90.0f);

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-3.60f, 0.0f, 4.27f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/rock_tallB.obj");
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 194.0f);

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2, 2, 2, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-9.7f, 0.0f, 7.87f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/rock_tallC.obj");
	
	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.65, 0.65, 0.65, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-13.0f, -0.5f, 7.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_fat_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5.0f, -0.5f, 16.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_detailed_dark.fbx");
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 109.0f);

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -0.5f, 18.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_cone_dark.fbx");
	
	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-4.7f, -0.5f, 25.6f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_oak_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(2.4f, -0.5f, 24.9f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_blocks_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(16.4f, -0.5f, 4.7f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/tree_blocks_dark.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(4.0f, -0.5f, 4.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/statue_columnDamaged.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12.0f, -0.5f, 4.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/statue_column.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12.0f, -0.5f, 12.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/statue_column.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(4.0f, -0.5f, 12.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/statue_column.fbx");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.5, 0.5, 0.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(8.0f, -0.5f, 8.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/statue_head.fbx");
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 180.0f);

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.5, 1.5, 1.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-8.0f, -0.5f, 15.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/log_stackLarge.obj");
	
	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.5, 1.5, 1.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-7.0f, -0.5f, 15.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/log_stackLarge.obj");

	backgroundObjects.push_back(new GameObject());
	backgroundObjects.back()->AddComponent<Rigidbody>();
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.5, 1.5, 1.5, 0.0f));
	backgroundObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -0.5f, 15.0f, 0.0f));
	backgroundObjects.back()->AddComponent<RenderComponent>();
	backgroundObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/log_stackLarge.obj");

}
void MAPScene::SetPath(int length_, bool vertical_, Vector4 startPos_)
{
	
	if (vertical_)
	{
		for (int i = 0; i < length_; i++)
		{
			pathObjects.push_back(new GameObject());
			pathObjects.back()->AddComponent<Rigidbody>();
			pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
			pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, i*2.5f, 2.0f)+startPos_);
			pathObjects.back()->AddComponent<RenderComponent>();
			pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathStraight.fbx");

		}
	}
	else
	{
		for (int i = 0; i < length_; i++)
		{
			pathObjects.push_back(new GameObject());
			pathObjects.back()->AddComponent<Rigidbody>();
			pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.25f, 0.25f, 0.25f, 0.0f));
			pathObjects.back()->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f),90.0f);
			pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(i*2.5f, 0.0f, 0.0f, 2.0f) + startPos_);
			pathObjects.back()->AddComponent<RenderComponent>();
			pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_pathStraight.fbx");

		}
	}
}
void  MAPScene::SetArea(int length_, int width, Vector4 startPos_)
{
	for (int x = 0; x < width; x++) {
		for (int i = 0; i < length_; i++)
		{
			pathObjects.push_back(new GameObject());
			pathObjects.back()->AddComponent<Rigidbody>();
			pathObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1, 1, 1, 0.0f));
			pathObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(x * 10.0f, 0.0f, i * 10.0f, 2.0f) + startPos_);
			pathObjects.back()->AddComponent<RenderComponent>();
			pathObjects.back()->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/ground_grass.fbx");

		}
	}
}
bool MAPScene::Initialize()
{
	return true;
}

void MAPScene::Update(const float deltaTime_)
{
	if (!PlayerTurn())
	{
		players.emplace(m_currentPlayer);
		m_currentPlayer = nullptr;
	}
}

bool MAPScene::PlayerTurn()
{
	if (!m_currentPlayer)
	{
		m_currentPlayer = players.front(); players.pop();
	}

	if (!hasRolled)
	{
		if (Input::Get().isKeyPressed(Key::R))
		{
			RollAction* roll = new RollAction(playerObjects[0], Dice(6));
			roll->Execute();
			numMoves = dieRoll = roll->GetRoll();
			hasRolled = true;
		}
	}

	int currentTile = m_currentPlayer->GetTile();

	if (Input::Get().isKeyPressed(Key::D))
	{
		if (currentTile - numMoves < 0)
		{
			int temp = numMoves - currentTile;
			m_currentPlayer->SetDestination(gameObjects.size() - temp);
		}
		else
		{
			m_currentPlayer->SetDestination(currentTile - numMoves);
		}
	}
	else if (Input::Get().isKeyPressed(Key::A))
	{
		m_currentPlayer->SetDestination((currentTile + numMoves) % gameObjects.size());
	}

	if (Input::Get().isKeyPressed(Key::Space) && currentTile != m_currentPlayer->GetDestination())
	{
		m_currentPlayer->Move(&gameObjects);
		int tileID = m_currentPlayer->GetDestination();

		tile = gameObjects[tileID]->Tag();

		numMoves = 0;
	}

	if (Input::Get().isKeyPressed(Key::Enter) || (numMoves == 0 && hasRolled))
	{
		dieRoll = 0;
		hasRolled = false;
		return false;
	}

	return true;
}


void MAPScene::UIRender()
{
	ImGui::Begin("Map UI");
	ImGui::Text("Press R to roll, space to choose where to move.");
	ImGui::Text("Dice Roll: %d", dieRoll);
	ImGui::Text("Movement: %d", numMoves);
	ImGui::Text("Landed on: %s", tile.c_str());

	ImGui::End();
}