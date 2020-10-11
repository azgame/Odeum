#include "RobScene.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/DynamicMovement.h"

#include "../Components/ComponentTest.h"

RobScene::RobScene() : Scene()
{
	newObject = new GameObject("Engine/Resources/Models/Cube.obj");
	newObject->SetPosition(Vector4(5.0f, 2.0f, 0.0f, 1.0f));
	object = new GameObject("Engine/Resources/Models/2nrtbod1out.obj");
	object->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent<SimplePhysics>();
	object->AddComponent <DynamicMovement>();
	//object->AddComponent <KinimaticMovement>();
	object->GetComponent<SimplePhysics>()->SetMass(1.0f);
	
	//object->GetComponent<SimplePhysics>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 0.0f, 1.0f), 0.02f);
	
	//Debug::Info("Creating rob", __FILENAME__, __LINE__);
	object->GetComponent<DynamicMovement>()->target = newObject;
	//object->GetComponent<KinimaticMovement>()->target = newObject;
	

	
	
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
	
	object->SetPosition(object->GetPosition() + Vector4(1.01f, 0.0f, 0.0f, 0.0f));
	//object->GetComponent<KinimaticMovement>()->shouldFlee = true;
    object->GetComponent<DynamicMovement>()->shouldFlee = true;
	}
	else
	{
		//object->GetComponent<KinimaticMovement>()->shouldFlee = false;
		object->GetComponent<DynamicMovement>()->shouldFlee = false;
	}
}


void RobScene::Render()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}
