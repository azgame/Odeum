#include "CollisionHandler.h"

#include "CollisionDetection.h"
#include "../Core/OdeumEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::colliders = std::vector<GameObject*>();
std::vector<GameObject*> CollisionHandler::previousCollisions = std::vector<GameObject*>();

CollisionHandler::CollisionHandler()
{
	colliders.reserve(10);
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

void CollisionHandler::Initialize()
{
	colliders.clear();
	previousCollisions.clear();


}

void CollisionHandler::AddObject(GameObject* go_)
{
	colliders.push_back(go_);
}

void CollisionHandler::MouseUpdate(DirectX::Mouse::State& mouse)
{
	D3D12_VIEWPORT viewport = OdeumEngine::GetInstance()->GetCamera()->GetViewPort();
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(DirectX::XMFLOAT2(mouse.x, mouse.y), DirectX::XMFLOAT2(viewport.Width, viewport.Height), OdeumEngine::GetInstance()->GetCamera());

	GameObject* hitResult = nullptr;
	float shortestDistance = FLT_MAX;

	for (auto g : colliders)
	{
		if (mouseRay.isColliding(&g->GetBoundingBox()))
		{
			if (mouseRay.t < shortestDistance) 
			{
				hitResult = g;
				shortestDistance = mouseRay.t;
			}
		}
	}

	if (hitResult)
		hitResult->SetHit(true, mouse);

	for (auto c : previousCollisions)
	{
		if (c != hitResult && c != nullptr)
			c->SetHit(false, mouse);

		c = nullptr;
	}
	previousCollisions.clear();

	if (hitResult)
		previousCollisions.push_back(hitResult);

	if (hitResult)
		hitResult->SetHit(false, mouse);
}

void CollisionHandler::Uninitialize()
{
	for (auto entry : colliders)
		entry = nullptr;
	colliders.clear();

	for (auto entry : previousCollisions)
		entry = nullptr;
	previousCollisions.clear();
}