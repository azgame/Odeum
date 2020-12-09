#include "RobScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/Rigidbody.h"
#include "../Components/Ai.h"
#include "../Engine/Math/CollisionHandler.h"
using namespace std;
RobScene::RobScene() : Scene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 25.0f));
	object = new GameObject(CubeShape, Colour(0.3f, 0.2, 0.5f));
	object->AddComponent<Ai>();
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
