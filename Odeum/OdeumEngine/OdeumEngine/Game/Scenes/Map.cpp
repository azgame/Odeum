#include "Map.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
//#include "../Components/PlayerEffectsComponent.h"
#include "../../Engine/CoreComponents/RenderComponent.h"
#include "../Game/Components/Player.h"





MAPScene::MAPScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.6f, 1.0f, 0.6f, 0.0f));
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>(); 
	playerObjects.back()->AddComponent<Player>(); 
	//playerObjects.back()->AddComponent<PlayerEffectsComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 0.0f));
	playerObjects.back()->SetTag("Player1");
	playerObjects.back()->GetComponent<Player>()->SetUp(Key::W);
	playerObjects.back()->GetComponent<Player>()->SetDown(Key::A);
	playerObjects.back()->GetComponent<Player>()->SetLeft(Key::S);
	playerObjects.back()->GetComponent<Player>()->SetRight(Key::D);

	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.6f, 1.0f, 0.6f, 0.0f));
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->AddComponent<Player>();
	//playerObjects.back()->AddComponent<PlayerEffectsComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 1.0f, 1.0f));
	playerObjects.back()->SetTag("Player2");
	playerObjects.back()->GetComponent<Player>()->SetUp(Key::I);
	playerObjects.back()->GetComponent<Player>()->SetDown(Key::K);
	playerObjects.back()->GetComponent<Player>()->SetLeft(Key::J);
	playerObjects.back()->GetComponent<Player>()->SetRight(Key::L);
	


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
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(0, 1, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-6.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(0, 2, 0);



	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-9.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(2, 3, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-12.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(3, 4, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-15.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(4, 5, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(5, 6, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//turning  on z axis the red  side steps

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 4.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(6, 7, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 8.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(7, 8, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 12.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(8, 9, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-18.0f, -0.4f, 16.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(9, 10, 0);

		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//on red side making the little square area
	


	//far left in square
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 21.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(10, 11, 0);




	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(11, 12, 0);
	
	///far right in square
	
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-16.9f, -0.4f, 21.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(12, 13, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-16.9f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(13, 14, 0);


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//middle stretch where red and blue connect

	//now this is the connecting side to middle but on brown steps


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-13.3f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(14, 15, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.1f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(15, 16, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-7.1f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(16, 17, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-3.9f, -0.4f, 26.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(17,18, 0);
	
	/// //////////////////////////////////////////////////////////////////////////////////////////////////
	/// continue path on red steps from square
	
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 31.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(12, 19, 0);

	
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 36.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(19, 20, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 42.5f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(20, 21, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 48.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(21, 22, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-20.0f, -0.4f, 54.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(22,23, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//the 4 square block

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-11.5f, -0.4f, 54.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(23, 24, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-11.5f, -0.4f, 45.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(24,25, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, -0.4f, 45.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(25,26, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, -0.4f, 54.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(26,27, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////
	
	//white square for red.
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -0.4f, 54.9f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 1.0f, 1.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(27,26,  0);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// the right steps blue
	//connected to start white spot

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");
	
	map.push(gameObjects.back());

	map.attachEdge(0,20, 0);



	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(20, 21, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(9.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(21, 22, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(12.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(22, 23, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(15.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(23, 24, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(24, 25, 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//turning  on z axis the blue side steps

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, 0.0f, 3.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(25, 26, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, 0.0f, 7.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(26, 27, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, 0.0f, 11.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(27, 28, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(18.0f, 0.0f, 15.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(28, 29, 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//on blue side making the blue quare side


	//far left side on blue square
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(17.20f, 0.0f, 20.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(29, 30, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(17.20f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(30, 31, 0);
	

	// far right side in square 

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 20.5f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(31, 32, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(32, 33, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//middle stretch where brown and blue connect
	



	//now this is the connecting side to middle but on blue steps

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(13.8f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(33, 34, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(10.5f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(34, 35, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(7.3f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(35, 36, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(4.0f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(36, 37, 0);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//continue path up on blue side far right 
	
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(4.0f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(37, 38, 0);
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2nd middle part
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 25.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 1.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(38, 39, 0);

	///////////////////////////////////////////////////////////////////
	//continue path on blue side from square
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 29.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(33, 40, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 35.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(40, 41, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 40.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(41, 42, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 47.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(42, 43, 0);



	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(20.5f, 0.0f, 54.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(43,44, 0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////square blue
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(11.5f, 0.0f, 54.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(44, 45, 0);


	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(11.5f, 0.0f, 45.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(45,46, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5.0f, 0.0f, 45.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(46,47, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5.0f, 0.0f, 54.9f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(47,48, 0);
	///////////////////////////////////////////////////////////////////////////////////////////////

	//white square for blue

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 44.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f,1.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(48,49, 0);


	//white 4th

	// NODE1
	/*gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(2.0f, 5.0f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.5, 0.5, 0.5));
	playerObjects.back()->SetTag("Node1");*/
}

MAPScene::~MAPScene() {
};

bool MAPScene::Initialize()
{
	return true;
}

void MAPScene::Update(const float deltaTime_) {
	/*if (Input::Get().isKeyPressed(Key::A)) {
		if (playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX() == -5.0f) {

		}
		else {
			playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>
				()->GetPosition().GetX() - 5.0f, 0.0f, -4.0f, 0.0f));
		}
	}
	if (Input::Get().isKeyPressed(Key::D)) {
		if (playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX() == 5.0f) {

		}
		else {
			playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>
				()->GetPosition().GetX() + 5.0f, 0.0f, -4.0f, 0.0f));
		}
	}*/

	int tempTurn = 1;
	// choose to move left, right, up or down based on the node they're standing on
	// uncomment playerTurn to turn on multiplayer
	switch (playerTurn) {
	case 1:
		if (tempTurn != turn && numMoves == 0) {
			if (Input::Get().isKeyPressed(playerObjects[0]->GetComponent<Player>()->GetLeft()) && numMoves > 0) {
				// logic here to find the gameobject the player is standing on

				// once the gameobject is found, go to the spot in the graph (the index should be the same) 

				// check for edges and check if the player can move left

				// move the player to the next node. (later on can pick up an item or something from a node)

				numMoves--;
				tempTurn--;
			}
			else if (Input::Get().isKeyPressed(playerObjects[0]->GetComponent<Player>()->GetRight()) && numMoves > 0) {

				tempTurn--;
			}
			else if (Input::Get().isKeyPressed(playerObjects[0]->GetComponent<Player>()->GetUp()) && numMoves > 0) {

				tempTurn--;
			}
			else if (Input::Get().isKeyPressed(playerObjects[0]->GetComponent<Player>()->GetDown()) && numMoves > 0) {

				tempTurn--;
			}
			else if (numMoves <= 0) {
				//playerTurn = 2;
				tempTurn = turn + 1;
			}
		}
		else {
			RollDice(6);
		}
		break;
	case 2:

		if (numMoves <= 0) {
			//playerTurn = 3;
		}
		break;
	case 3:

		//playerTurn = 4;
		break;
	case 4:

		//playerTurn = 5;
		break;
	default:
		playerTurn = 1;
		turn++;
		break;
	};
	if (Input::Get().isKeyPressed(Key::A)) {




		// Aidan's move logic (doesnt really work atm)
		//Action* move = new MoveAction(playerObjects[0], Vector2(0, 2));
		//move->Execute();
	}
}

//Spaces* MAPScene::findSpace(const std::string& name) {
//	for (size_t i = 0; i < spaces.size(); ++i)
//		if (spaces[i]->name == name)
//			return spaces[i];
//
//	return NULL;
//}
//
//
//
//Spaces* MAPScene::addSpace(const std::string& name) {
//	Spaces *spaceNode = new Spaces(name);
//
//	spaces.push_back(spaceNode);
//	graph.addNode(spaceNode);
//
//	return spaceNode;
//}

void MAPScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Map");

	ImGui::End();
}

void MAPScene::RollDice(int size){
	int min = 1;
	int max = size;
	int dieRoll;

	dieRoll = rand() % (max - min + 1) + min;
	
	numMoves = dieRoll;
}
