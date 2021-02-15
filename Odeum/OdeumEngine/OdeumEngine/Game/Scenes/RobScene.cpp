#include "RobScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/Rigidbody.h"
#include "../Components/Ai.h"
#include "../Engine/Math/CollisionHandler.h"
#include "../Components/RenderComponent.h"
using namespace std;
RobScene::RobScene() : Scene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 25.0f));
	
	CollisionHandler::GetInstance()->Initialize(1000.0f);
	object = new GameObject();
	object->AddComponent<Ai>();
	object->AddComponent<RenderComponent>();
	object->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cube.obj");
	//object->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	newObject = new GameObject();
	newObject->AddComponent<RenderComponent>();
	newObject->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cube.obj");
	//newObject->SetPosition(Vector4(5.0f, 0.0f, 5.0f, 1.0f));
	obstacle = new GameObject();
	obstacle->AddComponent<RenderComponent>();
	obstacle->GetComponent<RenderComponent>()->LoadModelFromFile("Engine/Resources/Models/Cube.obj");
	//obstacle->SetPosition(Vector4(2.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent<Rigidbody>();
	//object->AddComponent <DynamicMovement>();
	//object->GetComponent<DynamicMovement>()->shouldAvoid = true;

	obstacle->SetTag("Obstacle");
	object->SetTag("Player");
	//CollisionHandler::GetInstance()->AddObject(object);
	//CollisionHandler::GetInstance()->AddObject(newObject);
	//CollisionHandler::GetInstance()->AddObject(obstacle);

	Debug::Info("Creating rob", __FILENAME__, __LINE__);
	//object->GetComponent<DynamicMovement>()->target = newObject;
	object->GetComponent<Rigidbody>()->SetMass(1);

	teletime = 1000;
}

RobScene::~RobScene()
{
}

bool RobScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);

	return true;
}

void RobScene::Update(const float deltaTime_)
{
	


	object->Update(deltaTime_);
	
	teletime -= 1;
	
	




	if (Input::Get().isKeyPressed(Key::KeyCode::A))
	{

	}
	
}


void RobScene::UIRender()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}
