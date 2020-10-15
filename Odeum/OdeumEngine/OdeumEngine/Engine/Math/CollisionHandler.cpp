#include "CollisionHandler.h"

#include "CollisionDetection.h"
#include "../Core/OdeumEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::previousCollisions = std::vector<GameObject*>();
OctSpatialPartition* CollisionHandler::m_scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
	previousCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
	Uninitialize();
}

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr) {
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::Initialize(float worldsz_)
{
	m_scenePartition = new OctSpatialPartition(worldsz_);
}

void CollisionHandler::AddObject(GameObject* go_)
{
	m_scenePartition->AddObject(go_);
}

void CollisionHandler::MouseUpdate()
{
	if (Input::Get().isButtonClicked(Button1))
	{
		float width = OdeumEngine::Get().GetWindow().GetWidth();
		float height = OdeumEngine::Get().GetWindow().GetHeight();

		Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(Vector2(Input::Get().GetMouseX(),
			Input::Get().GetMouseY()), Vector2(width, height),
			OdeumEngine::Get().GetCamera());

		RayQuery(mouseRay);
	}
}

void CollisionHandler::RayQuery(Ray ray)
{
	if (m_scenePartition == nullptr) return;
	
	GameObject* hitResult = nullptr;
	hitResult = m_scenePartition->GetCollision(ray);

	if (hitResult)
		hitResult->SetHit(true);

	for (auto c : previousCollisions)
	{
		if (c != hitResult && c != nullptr)
			c->SetHit(false);
	}

	previousCollisions.clear();

	if (hitResult)
		previousCollisions.push_back(hitResult);
}

void CollisionHandler::Update()
{
	m_scenePartition->UpdatePartition();
}

void CollisionHandler::Uninitialize()
{
	SAFE_DELETE(m_scenePartition);

	for (auto entry : previousCollisions)
		entry = nullptr;
	previousCollisions.clear();
}