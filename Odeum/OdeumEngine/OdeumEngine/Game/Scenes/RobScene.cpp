#include "RobScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/Rigidbody.h"

#include "../Components/ComponentTest.h"

RobScene::RobScene() : Scene()
{
	newObject = new GameObject("Engine/Resources/Models/Cube.obj");
	object = new GameObject("Engine/Resources/Models/Cube.obj");
	object->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	newObject->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent<Rigidbody>();
	object->AddComponent <DynamicMovement>();
	object->AddComponent <KinematicMovement>();
	
	
	Debug::Info("Creating rob", __FILENAME__, __LINE__);
	object->GetComponent<DynamicMovement>()->target = newObject;
	//object->GetComponent<KinimaticMovement>()->target = newObject;
	object->GetComponent<Rigidbody>()->SetMass(1);

	
	
	teletime=1500;
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
	 if (teletime < 0)
	 {
		 teletime = 1500;
		 int a=(rand() % 10)-6;
		 int b=(rand() % 10)-6;
		 newObject->SetPosition(Vector4(a, 0.0f, b, 1.0f));
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
