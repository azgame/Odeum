#include "RobScene.h"
#include "../Components/KinimaticMovement.h"
RobScene::RobScene() : Scene()
{
	object = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.1f, 0.2f, 0.6f, 1.0f));
	object->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent <KinimaticMovement>();
	newObject = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.2f, 0.9f, 0.7f, 1.0f));
	newObject->SetPosition(Vector4(4.0f, 0.0f, 0.0f, 1.0f));
	plane = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.5f, 0.5f, 0.5f, 1.0f));
	plane->SetScale(Vector4(3.0f, 0.01f, 3.0f, 1.0f));
	plane->SetPosition(Vector4(0.0f, -1.0f, 1.0f, 1.0f));
	Debug::Info("Creating rob", __FILENAME__, __LINE__);
	object->GetComponent<KinimaticMovement>()->target = newObject;
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
	
	if (Input::Get().isKeyPressed(Key::KeyCode::A))
	{
	Debug::Info("Key A pressed in game scene", __FILENAME__, __LINE__);
	//object->SetPosition(object->GetPosition() + Vector4(1.01f, 0.0f, 0.0f, 0.0f));

	}
}


void RobScene::Render()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}
