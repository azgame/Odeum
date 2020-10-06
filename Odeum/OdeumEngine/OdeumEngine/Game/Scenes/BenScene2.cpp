#include "BenScene2.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/SimplePhysics.h"
BenScene2::BenScene2() : Scene()
{
	object = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.1f, 0.2f, 0.6f, 1.0f));
	object->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 1.0f));
	
	object2 = new GameObject("empty", ShapeTypes::CubeShape, Colour(1.0f, 0.7f, 0.6f, 1.0f));
	object2->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 1.0f));

	angle = 0.0f;
}

BenScene2::~BenScene2()
{
}

bool BenScene2::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);

	return true;
}

void BenScene2::Update(const float deltaTime_)
{
		
	object->SetPosition(object->GetPosition() + Vector4(0.002f, 0.0f, 0.0f, 0.0f));
	object->SetRotation(object->GetRotation() + Vector4(0.0f, 1.0f, 0.0f, 0.0f), angle);
	object2->SetPosition(object2->GetPosition() + Vector4(-0.002f, 0.0f, 0.0f, 0.0f));
	object2->SetRotation(object2->GetRotation() + Vector4(0.0f, 1.0f, 0.0f, 0.0f), -angle);
	angle += 0.002f;

	if (Input::Get().isKeyPressed(Key::KeyCode::A))
	{
		//Debug::Info("Key A pressed in game scene", __FILENAME__, __LINE__);
		//object->SetPosition(object->GetPosition() + Vector4(1.01f, 0.0f, 0.0f, 0.0f));

	}
}


void BenScene2::Render()
{
	// Don't add here, doesn't do anything
	//Don't tell me what to Do
}
