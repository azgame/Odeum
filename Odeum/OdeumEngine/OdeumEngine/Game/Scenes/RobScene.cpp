#include "RobScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/Rigidbody.h"
#include "../Engine/Math/CollisionHandler.h"
RobScene::RobScene() : Scene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 25.0f));
	CollisionHandler::GetInstance()->Initialize(1000.0f);
	object = new GameObject("Engine/Resources/Models/Cube.obj");
	object->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	newObject = new GameObject("Engine/Resources/Models/Cube.obj");
	newObject->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 1.0f));
	obstacle = new GameObject("Engine/Resources/Models/Cube.obj");
	obstacle->SetPosition(Vector4(2.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent<Rigidbody>();
	object->AddComponent <DynamicMovement>();
	object->AddComponent <KinematicMovement>();
	object->GetComponent<DynamicMovement>()->shouldAvoid = true;
	object->GetComponent<DynamicMovement>()->obstacles.insert(obstacle);
	
	//CollisionHandler::GetInstance()->AddObject(object);
	CollisionHandler::GetInstance()->AddObject(newObject);
	CollisionHandler::GetInstance()->AddObject(obstacle);
	std::cout << object->GetPosition().GetX() <<" "<< object->GetPosition().GetY() << std::endl;
	Debug::Info("Creating rob", __FILENAME__, __LINE__);
	object->GetComponent<DynamicMovement>()->target = newObject;
	//object->GetComponent<KinimaticMovement>()->target = newObject;
	object->GetComponent<Rigidbody>()->SetMass(1);

	
	
	teletime=1000;
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

	std::cout << object->GetPosition().GetX() << " " << object->GetPosition().GetY() << std::endl;
	if (shouldupdate)
	{
		object->Update(deltaTime_);
	}
	else
	{
	newObject->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 1.0f));
		shouldupdate = true;
	}
	 teletime -= 1;
	 if (teletime < 0)
	 {
		 teletime = 1000;
		 int a=(rand() % 10)-6;
		 int b=(rand() % 10)-6;
		// newObject->SetPosition(Vector4(a, 0.0f, b, 1.0f));
		 if (swapped==true)
		 {
			 newObject->SetPosition(Vector4(5, 0.0f, 2, 1.0f));
			 swapped = false;
		 }
		 else
		 {
			 swapped = true;
			 newObject->SetPosition(Vector4(-5, 0.0f, -2, 1.0f));
		 }
	 }

		 
	if (Input::Get().isKeyPressed(Key::KeyCode::A))
	{
	
	//object->SetPosition(object->GetPosition() + Vector4(1.01f, 0.0f, 0.0f, 0.0f));
	object->GetComponent<KinematicMovement>()->shouldFlee = true;
	object->GetComponent<DynamicMovement>()->shouldFlee = true;
	}
	else
	{
		object->GetComponent<KinematicMovement>()->shouldFlee = false;
		object->GetComponent<DynamicMovement>()->shouldFlee = false;
	}
}


void RobScene::UIRender()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}
