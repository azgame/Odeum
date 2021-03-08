#include "Map.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../../Engine/CoreComponents/Rigidbody.h"
#include "../../Engine/CoreComponents/RenderComponent.h"




MAPScene::MAPScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
	//were gonna tag all your nodes aka game objects via tag to work with our graph.

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 1.0f, 1.0f));
	gameObjects.back()->SetTag("Start");

	map.push(gameObjects.back());

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(3.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");
	map.push(gameObjects.back());

	map.attachEdge(0, 1, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-3.0f, -0.4f, 1.0f, 2.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Fight");

	map.push(gameObjects.back());
	map.attachEdge(0, 2, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(6.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(1, 3, 0);

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(1.0f, 0.2f, 1.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(9.0f, 0.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 1.0f));
	gameObjects.back()->SetTag("Item");

	map.push(gameObjects.back());
	map.attachEdge(1, 4, 0);

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