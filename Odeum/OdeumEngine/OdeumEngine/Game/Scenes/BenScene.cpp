#include "BenScene.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/SimplePhysics.h"
BenScene::BenScene() : Scene()
{
	object = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.1f, 0.2f, 0.6f, 1.0f));
	object->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	object->AddComponent<SimplePhysics>();
	//object->GetComponent<SimplePhysics>()->SetMass(1.0f);
	object->GetComponent<SimplePhysics>()->ApplyForce(Vector4(5.0f, 0.0f, 0.0f, 1.0f));
}

BenScene::~BenScene()
{
}

bool BenScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);

	return true;
}

void BenScene::Update(const float deltaTime_)
{


	object->Update(deltaTime_);

	if (Input::Get().isKeyPressed(Key::KeyCode::A))
	{
		Debug::Info("Key A pressed in game scene", __FILENAME__, __LINE__);
		//object->SetPosition(object->GetPosition() + Vector4(1.01f, 0.0f, 0.0f, 0.0f));

	}
}


void BenScene::Render()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}